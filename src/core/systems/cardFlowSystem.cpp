#include "core/systems/cardFlowSystem.h"

void CardFlowSystem::playCard(uint32_t cardId, std::vector<uint32_t> targets) {
}

void CardFlowSystem::discardLeftHand() {
    std::vector<Card> cards = _leftHand.clearOut();
    for (auto card : cards) {
        _discardPile.addCard(card);
        _cardDiscardedEventBus.emit(
            CardDiscardedEvent{card.getId(), _discardPile.size(), HandType::Left}
        );
    }
}

void CardFlowSystem::discardRightHand() {
    std::vector<Card> cards = _rightHand.clearOut();
    for (auto card : cards) {
        _discardPile.addCard(card);
        _cardDiscardedEventBus.emit(
            CardDiscardedEvent{card.getId(), _discardPile.size(), HandType::Right}
        );
    }
}

void CardFlowSystem::drawCard() {
    if (_drawPile.isEmpty()) {
        _discardPile.transfer(_drawPile);
        _drawPile.shuffle();
        _drawPileRefilledEventBus.emit(
            DrawPileRefilledEvent{_drawPile.size(), _discardPile.size()}
        );
    }

    if (_leftHand.isFull()) {
        if (_rightHand.isFull()) {
            Card discardedCard = _rightHand.popLast();
            _discardPile.addCard(discardedCard);
            _cardDiscardedEventBus.emit(
                CardDiscardedEvent{discardedCard.getId(), _discardPile.size(), HandType::Right}
            );
        }
        Card transferredCard = _leftHand.popLast();
        _rightHand.addCard(transferredCard);
        _cardTransferredToRightEventBus.emit(CardTransferredToRightEvent{transferredCard.getId()});
    }

    Card cardDrawn = _drawPile.popLast();
    _leftHand.addCard(cardDrawn);
    _cardDrawnEventBus.emit(CardDrawnEvent{cardDrawn, _drawPile.size()});
}