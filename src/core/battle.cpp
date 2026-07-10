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

void Battle::startBattle() {
    // Call-once: re-announcing would duplicate the UI's units, since
    // UnitPlacedEvent creates a widget per emission.
    if (_started)
        return;
    _started = true;

    _battleInfoChangedEventBus.emit({_info});
    _actionPointsChangedEventBus.emit({_actionPoints});
    _firePointsChangedEventBus.emit({_firePoints});
    _drawPileRefilledEventBus.emit({});
    for (const auto& [id, unit] : _units) {
        std::optional<HexCoord> at = _board.find(id);
        if (at)
            _unitPlacedEventBus.emit({id, *at, unit});
    }
    _captain.announce();
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
    BoardResult placed = _board.place(at, id, unit.getTeam(), BoardTileType::Unit);
    if (placed.isErr())
        return Result<uint32_t, BoardError>::err(placed.error());
    ++_nextUnitId;
    _units.emplace(id, unit);
    _unitPlacedEventBus.emit({id, at, unit});
    return Result<uint32_t, BoardError>::ok(id);
}

Unit* Battle::findUnit(uint32_t unitId) {
    auto it = _units.find(unitId);
    return it == _units.end() ? nullptr : &it->second;
}

BoardResult Battle::moveUnit(uint32_t unitId, HexCoord to) {
    Unit* unit = findUnit(unitId);
    std::optional<HexCoord> from = _board.find(unitId);
    if (!unit || !from)
        return BoardResult::err(BoardError::NoOccupant);
    BoardResult moved = _board.move(*from, to, unit->getTeam(), BoardTileType::Unit);
    if (moved.isErr())
        return moved;
    _unitMovedEventBus.emit({unitId, *from, to});
    return BoardResult::ok();
}

int Battle::dealDamageToUnit(uint32_t unitId, int amount) {
    Unit* unit = findUnit(unitId);
    if (!unit)
        return 0;
    int excess = unit->takeDamage(amount);
    _unitDamagedEventBus.emit({unitId, unit->getHealth(), unit->getArmor().getCurrentArmor()});

    if (!unit->isDead())
        return excess;
    std::optional<HexCoord> at = _board.find(unitId);
    if (at)
        _board.remove(*at);
    _units.erase(unitId);
    _unitDiedEventBus.emit({unitId, at.value_or(HexCoord{})});
    return excess;
}

void Battle::attackWithUnit(uint32_t attackerId) {
    Unit* attacker = findUnit(attackerId);
    std::optional<HexCoord> from = _board.find(attackerId);
    if (!attacker || !from)
        return;

    HexCoord dir = _board.attackDirectionFor(attacker->getTeam());

    // Overkill pierces: damage beyond a victim's armor + health carries on to
    // the next unit down the lane.
    int damage = attacker->getAttackPower();
    HexCoord at = *from;
    while (damage > 0) {
        const BoardTile* targetTile = _board.firstOccupiedTileAlong(at, dir);
        if (!targetTile)
            return;
        uint32_t targetId = *targetTile->occupantId();
        Unit* target = findUnit(targetId);
        if (!target || target->getTeam() == attacker->getTeam())
            return; // the lane is blocked by a friendly or a non-unit occupant
        at = targetTile->coord();
        damage = dealDamageToUnit(targetId, damage);
    }
}

void Battle::defendWithUnit(uint32_t defenderId) {
    Unit* unit = findUnit(defenderId);
    if (!unit)
        return;
    unit->defend();
    _unitDefendedEventBus.emit({defenderId, unit->getArmor().getCurrentArmor()});
}

void Battle::healUnit(uint32_t unitId, int amount) {
    Unit* unit = findUnit(unitId);
    if (!unit)
        return;
    unit->heal(amount);
    _unitHealedEventBus.emit({unitId, unit->getHealth()});
}
