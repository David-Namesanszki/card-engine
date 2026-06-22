#pragma once

#include <string>
#include "cardPile.h"
#include "eventBus.h"
#include "events/cardDrawnEvent.h"
#include "events/cardTransferredToRightEvent.h"
#include "events/cardDiscardedEvent.h"
#include "events/drawPileRefilledEvent.h"
#include "raid/captain.h"

class Battle {
public:
    Battle(Captain& captain, int firePoints, const CardPile& deck);
    void startPlayerTurn();
    void endPlayerTurn();
    void drawCard();
    void playCard();
    void discardLeftHand();
    void discardRightHand();

    const CardPile& getDrawPile()    const { return _drawPile; }
    const CardPile& getDiscardPile() const { return _discardPile; }
    const CardPile& getLeftHand()    const { return _leftHand; }
    const CardPile& getRightHand()   const { return _rightHand; }
private:
    Captain&  _captain;
    CardPile  _leftHand{5};
    CardPile  _rightHand{5};
    CardPile  _drawPile{-1};
    CardPile  _discardPile{-1};
    int       _actionPoints = 0;
    int       _firePoints;

    EventBus<CardDrawnEvent>              _cardDrawnEventBus;
    EventBus<CardTransferredToRightEvent> _cardTransferredToRightEventBus;
    EventBus<CardDiscardedEvent>          _cardDiscardedEventBus;
    EventBus<DrawPileRefilledEvent>       _drawPileRefilledEventBus;

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
