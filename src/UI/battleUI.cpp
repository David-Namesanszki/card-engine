#include "UI/battleUI.h"
#include "raylib.h"
#include "engine/easing.h"
#include <algorithm>
#include <random>
#include <vector>

BattleUI::BattleUI(
    BattleUIConfig config,
    HandUIConfig leftHandConfig,
    HandUIConfig rightHandConfig,
    CardPileUIConfig discardPileConfig,
    CardPileUIConfig drawPileConfig
)
    : _config(config), _drawPile(drawPileConfig), _discardPile(discardPileConfig),
      _leftHand(leftHandConfig), _rightHand(rightHandConfig) {
}

void BattleUI::drawCard(uint32_t cardId) {
    CardUI c(cardId);
    c.transform = _drawPile.getAnchorPoint().transform;
    _cards.push_back(c);

    _leftHand.addCardId(cardId, 0);
    const std::list<uint32_t>& cards = _leftHand.getCardIds();
    for (auto c : cards) {
        auto it = std::find(_cards.begin(), _cards.end(), [c](const CardUI& card) {
            return card.id() == c;
        });
        if (it == _cards.end()) {
            // problem
        }
        const AnchorPoint& pos = _leftHand.getPosition(c);
        it->animateTo(pos.transform, 2.0);
    }
}

void BattleUI::discardFromLeftHand(uint32_t cardId) {
    _leftHand.removeCardId(cardId);
    const std::list<uint32_t>& cards = _leftHand.getCardIds();
    for (auto c : cards) {
        auto it = std::find(_cards.begin(), _cards.end(), [c](const CardUI& card) {
            return card.id() == c;
        });
        if (it == _cards.end()) {
            // problem
        }
        const AnchorPoint& pos = _leftHand.getPosition(c);
        it->animateTo(pos.transform, 2.0);
    }
}

void BattleUI::discardFromRightHand(uint32_t cardId) {
    _leftHand.removeCardId(cardId);
    const std::list<uint32_t>& cards = _leftHand.getCardIds();
    for (auto c : cards) {
        auto it = std::find(_cards.begin(), _cards.end(), [c](const CardUI& card) {
            return card.id() == c;
        });
        if (it == _cards.end()) {
            // problem
        }
        const AnchorPoint& pos = _rightHand.getPosition(c);
        it->animateTo(pos.transform, 2.0);
    }
}
