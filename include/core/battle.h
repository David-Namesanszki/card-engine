#pragma once

#include <string>
#include <functional>
#include <optional>
#include <unordered_map>
#include "core/cardPile.h"
#include "core/board.h"
#include "core/unit.h"
#include "core/events/unitPlacedEvent.h"
#include "core/events/unitMovedEvent.h"
#include "core/events/unitDamagedEvent.h"
#include "core/events/unitHealedEvent.h"
#include "core/events/unitDiedEvent.h"
#include "core/events/unitDefendedEvent.h"
#include "engine/eventBus.h"
#include "core/events/cardDrawnEvent.h"
#include "core/events/cardTransferredToRightEvent.h"
#include "core/events/cardDiscardedEvent.h"
#include "core/events/drawPileRefilledEvent.h"
#include "core/events/battleInfoChangedEvent.h"
#include "core/events/actionPointsChangedEvent.h"
#include "core/events/firePointsChangedEvent.h"
#include "core/captain.h"
#include "core/targetReq.h"
#include "engine/result.h"
#include "core/cardPlayError.h"
#include "core/battleInfo.h"
#include "core/actionPoints.h"

class Battle {
  public:
    Battle(
        Captain& captain,
        int firePoints,
        const CardPile& deck,
        BattleDifficultyType difficulty,
        int maxActionPoints,
        Board board
    );
    // Announces the battle's current state through the normal event buses.
    // Call once, after subscribers are wired: the UI initialises from these
    // events the same way it stays current afterwards.
    void startBattle();
    void startPlayerTurn();
    void endPlayerTurn();
    void drawCard();

    Result<TargetReq, PlayError> tryPlayCard(uint32_t cardId);
    void playCard(uint32_t cardId, std::vector<uint32_t> targets);
    void discardLeftHand();
    void discardRightHand();
    void switchTurn();
    void refillActionPoints();
    void incrementTurnCounter();

    void healUnit(uint32_t unitId, int amount);
    void attackWithUnit(uint32_t attackerId);
    void defendWithUnit(uint32_t defenderId);
    // Returns the overkill damage the unit could not absorb (0 if it survived
    // or the id is stale) — attackWithUnit carries it down the lane.
    int dealDamageToUnit(uint32_t unitId, int amount);
    Result<uint32_t, BoardError> placeUnit(const Unit& unit, HexCoord at);
    BoardResult moveUnit(uint32_t unitId, HexCoord to);

    const CardPile& getDrawPile() const {
        return _drawPile;
    }
    const CardPile& getDiscardPile() const {
        return _discardPile;
    }
    const CardPile& getLeftHand() const {
        return _leftHand;
    }
    const CardPile& getRightHand() const {
        return _rightHand;
    }
    const BattleInfo& getInfo() const {
        return _info;
    }
    const ActionPoints& getActionPoints() const {
        return _actionPoints;
    }
    const Board& getBoard() const {
        return _board;
    }
    const Unit* getUnit(uint32_t unitId) const {
        auto it = _units.find(unitId);
        return it == _units.end() ? nullptr : &it->second;
    }

    void onCardDrawn(std::function<void(CardDrawnEvent)> cb) {
        _cardDrawnEventBus.subscribe(cb);
    }
    void onCardDiscarded(std::function<void(CardDiscardedEvent)> cb) {
        _cardDiscardedEventBus.subscribe(cb);
    }
    void onCardTransferredToRight(std::function<void(CardTransferredToRightEvent)> cb) {
        _cardTransferredToRightEventBus.subscribe(cb);
    }
    void onDrawPileRefilled(std::function<void(DrawPileRefilledEvent)> cb) {
        _drawPileRefilledEventBus.subscribe(cb);
    }
    void onBattleInfoChanged(std::function<void(BattleInfoChangedEvent)> cb) {
        _battleInfoChangedEventBus.subscribe(cb);
    }
    void onActionPointsChanged(std::function<void(ActionPointsChangedEvent)> cb) {
        _actionPointsChangedEventBus.subscribe(cb);
    }
    void onFirePointsChanged(std::function<void(FirePointsChangedEvent)> cb) {
        _firePointsChangedEventBus.subscribe(cb);
    }
    void onUnitPlaced(std::function<void(UnitPlacedEvent)> cb) {
        _unitPlacedEventBus.subscribe(cb);
    }
    void onUnitMoved(std::function<void(UnitMovedEvent)> cb) {
        _unitMovedEventBus.subscribe(cb);
    }
    void onUnitDamaged(std::function<void(UnitDamagedEvent)> cb) {
        _unitDamagedEventBus.subscribe(cb);
    }
    void onUnitHealed(std::function<void(UnitHealedEvent)> cb) {
        _unitHealedEventBus.subscribe(cb);
    }
    void onUnitDied(std::function<void(UnitDiedEvent)> cb) {
        _unitDiedEventBus.subscribe(cb);
    }
    void onUnitDefended(std::function<void(UnitDefendedEvent)> cb) {
        _unitDefendedEventBus.subscribe(cb);
    }

  private:
    // Mutable twin of getUnit for Battle's own rule code.
    Unit* findUnit(uint32_t unitId);

    Captain& _captain;
    CardPile _leftHand{5};
    CardPile _rightHand{5};
    CardPile _drawPile{-1};
    CardPile _discardPile{-1};
    ActionPoints _actionPoints;
    int _firePoints;
    BattleInfo _info;
    bool _started = false;
    Board _board;
    std::unordered_map<uint32_t, Unit> _units;
    uint32_t _nextUnitId = 0;

    EventBus<CardDrawnEvent> _cardDrawnEventBus;
    EventBus<CardTransferredToRightEvent> _cardTransferredToRightEventBus;
    EventBus<CardDiscardedEvent> _cardDiscardedEventBus;
    EventBus<DrawPileRefilledEvent> _drawPileRefilledEventBus;
    EventBus<BattleInfoChangedEvent> _battleInfoChangedEventBus;
    EventBus<ActionPointsChangedEvent> _actionPointsChangedEventBus;
    EventBus<FirePointsChangedEvent> _firePointsChangedEventBus;
    EventBus<UnitPlacedEvent> _unitPlacedEventBus;
    EventBus<UnitMovedEvent> _unitMovedEventBus;
    EventBus<UnitDamagedEvent> _unitDamagedEventBus;
    EventBus<UnitHealedEvent> _unitHealedEventBus;
    EventBus<UnitDiedEvent> _unitDiedEventBus;
    EventBus<UnitDefendedEvent> _unitDefendedEventBus;
};
