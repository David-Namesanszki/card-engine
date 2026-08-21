#pragma once

#include "engine/eventBus.h"
#include "core/cardPile.h"
#include "core/events/cardDrawnEvent.h"
#include "core/events/cardTransferredToRightEvent.h"
#include "core/events/cardDiscardedEvent.h"
#include "core/events/drawPileRefilledEvent.h"

class CardFlowSystem {
  public:
    CardFlowSystem(CardPile cards, size_t leftHandSize, size_t rightHandSize)
        : _leftHand(leftHandSize),
          _rightHand(rightHandSize),
          _drawPile(-1),
          _discardPile(-1) {
        cards.transfer(_drawPile);
    };
    void playCard(uint32_t cardId, std::vector<uint32_t> targets);
    void discardLeftHand();
    void discardRightHand();
    void drawCard();

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
    CardPile _leftHand;
    CardPile _rightHand;
    CardPile _drawPile;
    CardPile _discardPile;

    EventBus<CardDrawnEvent> _cardDrawnEventBus;
    EventBus<CardTransferredToRightEvent> _cardTransferredToRightEventBus;
    EventBus<CardDiscardedEvent> _cardDiscardedEventBus;
    EventBus<DrawPileRefilledEvent> _drawPileRefilledEventBus;
};