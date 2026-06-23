#include "core/battle.h"
#include <vector>

constexpr int DRAW_SIZE = 6;

Battle::Battle(Captain& captain, int firePoints, const CardPile& deck)
    : _captain(captain), _actionPoints(0), _firePoints(firePoints), _drawPile(deck) {}

void Battle::startPlayerTurn() {
    for (size_t i = 0; i < DRAW_SIZE; i++) {
        drawCard();
    }
    
    // draw x cards -> base + captain effects + rune effects
    // refill action points
    // increment turn counter;
}

void Battle::endPlayerTurn() {
    // discard  cards
    // refill action points
    // increment turn counter;
    discardLeftHand();
    discardRightHand();
}

void Battle::drawCard() {
    if (_drawPile.isEmpty()) {
        _discardPile.transfer(_drawPile);
        _drawPile.shuffle();
        _drawPileRefilledEventBus.emit({_drawPileRefillSound, _drawPileRefillAnimation});
    }

    if (_leftHand.isFull()) {
        if (_rightHand.isFull()) {
            uint32_t discardedCard = _rightHand.popLast();
            _discardPile.addCard(discardedCard);
            _cardDiscardedEventBus.emit({discardedCard, HandType::Right, _cardDiscardSound, _cardDiscardAnimation});
        }
        uint32_t transferredCard = _leftHand.popLast();
        _rightHand.addCard(transferredCard);
        _cardTransferredToRightEventBus.emit({transferredCard, _cardTransferAnimation, _cardTransferSound});
    }

    uint32_t cardDrawn = _drawPile.popLast();
    _leftHand.addCard(cardDrawn);
    _cardDrawnEventBus.emit({cardDrawn, _cardDrawSound, _cardDrawAnimation});
}

void Battle::discardLeftHand() {
    std::vector<uint32_t> cards = _leftHand.clearOut();
    for (auto cardId : cards) {
        _discardPile.addCard(cardId);
        _cardDiscardedEventBus.emit({cardId, HandType::Left, "", ""});
    }
}

void Battle::discardRightHand() {
    std::vector<uint32_t> cards = _rightHand.clearOut();
    for (auto cardId : cards) {
        _discardPile.addCard(cardId);
        _cardDiscardedEventBus.emit({cardId, HandType::Right, "", ""});
    }
}

void Battle::playCard() {
    // TODO: apply selected card's effect
}
