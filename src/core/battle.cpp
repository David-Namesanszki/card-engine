#include "core/battle.h"
#include <vector>
#include <iostream>

constexpr int DRAW_SIZE = 6;

Battle::Battle(
    Captain& captain,
    int firePoints,
    const CardPile& deck,
    BattleDifficultyType difficulty,
    int maxActionPoints,
    Board board
)
    : _captain(captain),
      _actionPoints{0, maxActionPoints},
      _firePoints(firePoints),
      _drawPile(deck),
      _info{0, TurnType::Enemy, difficulty},
      _board(std::move(board)) {
}

void Battle::startPlayerTurn() {
    switchTurn();
    for (size_t i = 0; i < DRAW_SIZE; i++) {
        drawCard();
    }

    refillActionPoints();
    incrementTurnCounter();
}

void Battle::incrementTurnCounter() {
    _info.turnCount++;
    _battleInfoChangedEventBus.emit({_info});
}

void Battle::refillActionPoints() {
    _actionPoints.refill();
    _actionPointsChangedEventBus.emit({_actionPoints});
}

void Battle::switchTurn() {
    _info.turnType = _info.turnType == TurnType::Player ? TurnType::Enemy : TurnType::Player;
    _battleInfoChangedEventBus.emit({_info});
}

void Battle::endPlayerTurn() {
    discardLeftHand();
    discardRightHand();
    switchTurn();
}

void Battle::drawCard() {
    if (_drawPile.isEmpty()) {
        _discardPile.transfer(_drawPile);
        _drawPile.shuffle();
        _drawPileRefilledEventBus.emit({});
    }

    if (_leftHand.isFull()) {
        if (_rightHand.isFull()) {
            uint32_t discardedCard = _rightHand.popLast();
            _discardPile.addCard(discardedCard);
            _cardDiscardedEventBus.emit({discardedCard, HandType::Right});
        }
        uint32_t transferredCard = _leftHand.popLast();
        _rightHand.addCard(transferredCard);
        _cardTransferredToRightEventBus.emit({transferredCard});
    }

    uint32_t cardDrawn = _drawPile.popLast();
    _leftHand.addCard(cardDrawn);
    _cardDrawnEventBus.emit({cardDrawn});
}

void Battle::discardLeftHand() {
    std::vector<uint32_t> cards = _leftHand.clearOut();
    for (auto cardId : cards) {
        _discardPile.addCard(cardId);
        _cardDiscardedEventBus.emit({cardId, HandType::Left});
    }
}

void Battle::discardRightHand() {
    std::vector<uint32_t> cards = _rightHand.clearOut();
    for (auto cardId : cards) {
        _discardPile.addCard(cardId);
        _cardDiscardedEventBus.emit({cardId, HandType::Right});
    }
}

void Battle::playCard(uint32_t cardId, std::vector<uint32_t> targets) {
    // apply selected card's effect
    std::cout << "playCard";
}

Result<TargetReq, PlayError> Battle::tryPlayCard(uint32_t cardId) {
    return Result<TargetReq, PlayError>::ok({1, {5, 6, 7}});
}

Result<uint32_t, BoardError> Battle::placeUnit(const Unit& unit, HexCoord at) {
    const uint32_t id = _nextUnitId;
    BoardResult placed = _board.place(at, id, unit.team, BoardTileType::Unit);
    if (placed.isErr())
        return Result<uint32_t, BoardError>::err(placed.error());
    ++_nextUnitId;
    _units.emplace(id, unit);
    _unitPlacedEventBus.emit({id, at});
    return Result<uint32_t, BoardError>::ok(id);
}

BoardResult Battle::moveUnit(uint32_t unitId, HexCoord to) {
    auto unitIt = _units.find(unitId);
    std::optional<HexCoord> from = _board.find(unitId);
    if (unitIt == _units.end() || !from)
        return BoardResult::err(BoardError::NoOccupant);
    BoardResult moved = _board.move(*from, to, unitIt->second.team, BoardTileType::Unit);
    if (moved.isErr())
        return moved;
    _unitMovedEventBus.emit({unitId, *from, to});
    return BoardResult::ok();
}
