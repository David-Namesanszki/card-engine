#pragma once

#include <string>
#include <functional>
#include "core/cardPile.h"
#include "engine/eventBus.h"
#include "core/events/cardDrawnEvent.h"
#include "core/events/cardTransferredToRightEvent.h"
#include "core/events/cardDiscardedEvent.h"
#include "core/events/drawPileRefilledEvent.h"
#include "core/captain.h"
#include "core/targetReq.h"
#include "engine/result.h"
#include "core/cardPlayError.h"

class Battle {
  public:
    Battle(Captain& captain, int firePoints, const CardPile& deck);
    void startPlayerTurn();
    void endPlayerTurn();
    void drawCard();
    Result<TargetReq, PlayError> tryPlayCard(uint32_t cardId);
    void playCard(uint32_t cardId, std::vector<uint32_t> targets);
    void discardLeftHand();
    void discardRightHand();

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

  private:
    Captain& _captain;
    CardPile _leftHand{5};
    CardPile _rightHand{5};
    CardPile _drawPile{-1};
    CardPile _discardPile{-1};
    int _actionPoints = 0;
    int _firePoints;

    EventBus<CardDrawnEvent> _cardDrawnEventBus;
    EventBus<CardTransferredToRightEvent> _cardTransferredToRightEventBus;
    EventBus<CardDiscardedEvent> _cardDiscardedEventBus;
    EventBus<DrawPileRefilledEvent> _drawPileRefilledEventBus;

    std::string _leftHandCardPlayAnimation;
    std::string _rightHandCardPlayAnimation;
    std::string _cardDrawAnimation;
    std::string _cardTransferAnimation;
    std::string _cardDiscardAnimation;
    std::string _drawPileRefillAnimation;

    std::string _leftHandCardPlaySound;
    std::string _cardDrawSound;
    std::string _cardTransferSound;
    std::string _cardDiscardSound;
    std::string _drawPileRefillSound;
};
