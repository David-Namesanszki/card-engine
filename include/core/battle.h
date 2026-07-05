#pragma once

#include <string>
#include <functional>
#include "core/cardPile.h"
#include "engine/eventBus.h"
#include "core/events/cardDrawnEvent.h"
#include "core/events/cardTransferredToRightEvent.h"
#include "core/events/cardDiscardedEvent.h"
#include "core/events/drawPileRefilledEvent.h"
#include "core/events/battleInfoChangedEvent.h"
#include "core/events/actionPointsChangedEvent.h"
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
        int maxActionPoints
    );
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

  private:
    Captain& _captain;
    CardPile _leftHand{5};
    CardPile _rightHand{5};
    CardPile _drawPile{-1};
    CardPile _discardPile{-1};
    ActionPoints _actionPoints;
    int _firePoints;
    BattleInfo _info;

    EventBus<CardDrawnEvent> _cardDrawnEventBus;
    EventBus<CardTransferredToRightEvent> _cardTransferredToRightEventBus;
    EventBus<CardDiscardedEvent> _cardDiscardedEventBus;
    EventBus<DrawPileRefilledEvent> _drawPileRefilledEventBus;
    EventBus<BattleInfoChangedEvent> _battleInfoChangedEventBus;
    EventBus<ActionPointsChangedEvent> _actionPointsChangedEventBus;
};
