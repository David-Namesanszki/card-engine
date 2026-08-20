#include "core/battle.h"
#include <vector>
#include <iostream>

constexpr int DRAW_SIZE = 6;

Battle::Battle(Captain& captain, BattleDifficultyType difficulty, Board board)
    : _captain(captain),
      _actionPoints{0, captain.getMaxActionPointCount()},
      _info{0, TurnType::Enemy, difficulty},
      _board(std::move(board)) {
}

void Battle::startBattle() {
    if (_started)
        return;
    _started = true;

    for (const Card& card : _captain.getDeck().getCards()) {
        _drawPile.addCard(card.getId());
    }

    _drawPile.shuffle();

    std::vector<uint32_t> boardPieceIds = _board.getBoardPieceIds();
    std::vector<Unit> units;
    std::vector<Construction> constructions;

    for (uint32_t id : boardPieceIds) {
        auto* boardPiece = _captain.getRoster().getBoardPiece(id);

        if (auto* unit = dynamic_cast<Unit*>(boardPiece)) {
            units.push_back(*unit);
        } else if (auto* construction = dynamic_cast<Construction*>(boardPiece)) {
            constructions.push_back(*construction);
        } else {
            throw;
        }
    }

    _battleStartedEventBus.emit(
        {_captain.getHealth(),
         _captain.getDeck(),
         _board,
         std::move(units),
         std::move(constructions),
         _actionPoints,
         _info}
    );
}

void Battle::startTacticalPhase() {
    _tacticalPhaseStartedEventBus.emit(
        {_captain.getHealth(),
         _captain.getRoster(),
         _captain.getDeck(),
         _board,
         _captain.getMaxActionPointCount(),
         _captain.getFirePointCount(),
         _info}
    );
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

void Battle::placeBoardPiece(uint32_t id, HexCoord at) {
    const BoardPiece* boardPiece = _captain.getRoster().getBoardPiece(id);

    _board.place(at, boardPiece->getId(), boardPiece->getTeam(), boardPiece->getType());
    _boardPiecePlacedEventBus.emit({id, at});
}

void Battle::moveBoardPiece(uint32_t id, HexCoord to) {
    std::optional<HexCoord> from = _board.find(id);

    if (!from.has_value()) {
        return;
    }

    _board.move(*from, to);
    _boardPieceMovedEventBus.emit({id, *from, to});
}

int Battle::dealDamageToUnit(uint32_t id, int amount) {
    Unit* unit = dynamic_cast<Unit*>(_captain.getRoster().getBoardPiece(id));
    if (!unit)
        return 0;

    int excess = unit->takeDamage(amount);
    _unitDamagedEventBus.emit({id, unit->getHealth(), unit->getArmor().getCurrentArmor()});
    if (!unit->isDead())
        return excess;
    std::optional<HexCoord> at = _board.find(id);
    if (at.has_value()) {
        _board.remove(*at);
        _unitDiedEventBus.emit({id, *at});
    }

    return excess;
}

void Battle::attackWithUnit(uint32_t id) {
    std::optional<HexCoord> from = _board.find(id);
    Unit* attacker = dynamic_cast<Unit*>(_captain.getRoster().getBoardPiece(id));

    if (!attacker || !from)
        return;

    HexCoord dir = _board.attackDirectionFor(attacker->getTeam());
    int damage = attacker->getAttackPower();
    HexCoord at = *from;

    while (damage > 0) {
        const BoardPieceBoardTile* targetTile = _board.firstOccupiedTileAlong(at, dir);
        if (!targetTile)
            return;

        uint32_t targetId = *targetTile->getBoardPieceId();
        Unit* target = dynamic_cast<Unit*>(_captain.getRoster().getBoardPiece(targetId));
        if (!target || target->getTeam() == attacker->getTeam())
            return; // the lane is blocked by a friendly or a non-unit occupant
        at = targetTile->coord();
        damage = dealDamageToUnit(targetId, damage);
    }
}

void Battle::defendWithUnit(uint32_t id) {
    Unit* defender = dynamic_cast<Unit*>(_captain.getRoster().getBoardPiece(id));
    if (!defender)
        return;
    defender->defend();
    _unitDefendedEventBus.emit({id, defender->getArmor().getCurrentArmor()});
}

void Battle::healUnit(uint32_t id, int amount) {
    Unit* healed = dynamic_cast<Unit*>(_captain.getRoster().getBoardPiece(id));
    if (!healed)
        return;
    healed->heal(amount);
    _unitHealedEventBus.emit({id, healed->getHealth()});
}
