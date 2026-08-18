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

    std::vector<uint32_t> placedUnitIds = _board.getPlacedOccupantIds(BoardTileType::Unit);
    std::vector<uint32_t> placedConstructionIds =
        _board.getPlacedOccupantIds(BoardTileType::Construction);

    std::vector<Unit> selectedUnits;
    selectedUnits.reserve(placedUnitIds.size());
    for (uint32_t id : placedUnitIds)
        selectedUnits.push_back(*_captain.getRoster().getUnit(id));

    std::vector<Construction> selectedConstructions;
    selectedConstructions.reserve(placedConstructionIds.size());
    for (uint32_t id : placedConstructionIds)
        selectedConstructions.push_back(*_captain.getRoster().getConstruction(id));

    _battleStartedEventBus.emit(
        {_captain.getHealth(),
         _captain.getDeck(),
         _board,
         std::move(selectedUnits),
         std::move(selectedConstructions),
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

void Battle::placeUnit(uint32_t unitId, HexCoord at) {
    const Unit* unit = _captain.getRoster().getUnit(unitId);

    if (unit == nullptr)
        return;

    _board.place(at, unit->getId(), unit->getTeam(), BoardTileType::Unit);
    _unitPlacedEventBus.emit({unit->getId(), at, *unit});
}

BoardResult Battle::moveUnit(uint32_t unitId, HexCoord to) {
    const Unit* unit = _captain.getRoster().getUnit(unitId);

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
    Unit* unit = _captain.getRoster().getUnit(unitId);
    if (!unit)
        return 0;
    int excess = unit->takeDamage(amount);
    _unitDamagedEventBus.emit({unitId, unit->getHealth(), unit->getArmor().getCurrentArmor()});

    if (!unit->isDead())
        return excess;
    std::optional<HexCoord> at = _board.find(unitId);
    if (at)
        _board.remove(*at);

    _units.erase(std::find(_units.begin(), _units.end(), unitId));
    _unitDiedEventBus.emit({unitId, at.value_or(HexCoord{})});
    return excess;
}

void Battle::attackWithUnit(uint32_t attackerId) {
    Unit* attacker = _captain.getRoster().getUnit(attackerId);
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
        Unit* target = _captain.getRoster().getUnit(targetId);
        if (!target || target->getTeam() == attacker->getTeam())
            return; // the lane is blocked by a friendly or a non-unit occupant
        at = targetTile->coord();
        damage = dealDamageToUnit(targetId, damage);
    }
}

void Battle::defendWithUnit(uint32_t defenderId) {
    Unit* unit = _captain.getRoster().getUnit(defenderId);
    if (!unit)
        return;
    unit->defend();
    _unitDefendedEventBus.emit({defenderId, unit->getArmor().getCurrentArmor()});
}

void Battle::healUnit(uint32_t unitId, int amount) {
    Unit* unit = _captain.getRoster().getUnit(unitId);
    if (!unit)
        return;
    unit->heal(amount);
    _unitHealedEventBus.emit({unitId, unit->getHealth()});
}
