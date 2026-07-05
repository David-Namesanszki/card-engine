#include "core/battle.h"
#include <vector>
#include <iostream>

constexpr int DRAW_SIZE = 6;

Battle::Battle(
    Captain& captain,
    int firePoints,
    const CardPile& deck,
    BattleDifficultyType difficulty,
    int maxActionPoints
)
    : _captain(captain),
      _actionPoints{0, maxActionPoints},
      _firePoints(firePoints),
      _drawPile(deck),
      _info{0, TurnType::Enemy, difficulty} {
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
