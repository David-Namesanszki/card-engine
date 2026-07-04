#include "UI/battleUI.h"
#include "raylib.h"
#include "engine/easing.h"
#include <algorithm>
#include <random>
#include <vector>

BattleUI::BattleUI(
    BattleUIConfig config,
    BoardUIConfig boardConfig,
    FireResourcePanelUIConfig firePanelConfig,
    ActionPointsPanelUIConfig actionPointsConfig,
    ProgressPanelUIConfig progressPanelConfig,
    BattleInfoPanelUIConfig battleInfoPanelConfig,
    ButtonUIConfig endTurnButtonConfig,
    HandUIConfig leftHandConfig,
    HandUIConfig rightHandConfig,
    CardPileUIConfig discardPileConfig,
    CardPileUIConfig drawPileConfig
)
    : _config(config),
      _board(boardConfig),
      _drawPile(drawPileConfig),
      _discardPile(discardPileConfig),
      _leftHand(leftHandConfig),
      _rightHand(rightHandConfig),
      _firePanel(firePanelConfig),
      _actionPointsPanel(actionPointsConfig),
      _progressPanel(progressPanelConfig),
      _battleInfoPanel(battleInfoPanelConfig),
      _endTurnButton(endTurnButtonConfig) {
}

#pragma region Domain

void BattleUI::refillDrawPile(int drawPileSize, int discardPileSize) {
    _drawPile.setCount(drawPileSize);
    _discardPile.setCount(discardPileSize);
}

void BattleUI::setFireCount(int fireCount) {
    _firePanel.setFireCount(fireCount);
}

void BattleUI::setActionPointsSpent(int spentCount) {
    _actionPointsPanel.setSpent(spentCount);
}

void BattleUI::transferCardToRight(uint32_t cardId) {
    _leftHand.removeCardId(cardId);
    reorganizeHand(_leftHand);
    _rightHand.addCardId(cardId, 0);
    reorganizeHand(_rightHand);
}

void BattleUI::drawCard(uint32_t cardId, std::string texture, int drawPileSize) {
    addCard(cardId, texture);
    _drawPile.setCount(drawPileSize);
    _leftHand.addCardId(cardId, 0);
    reorganizeHand(_leftHand);
}

void BattleUI::discardFromLeftHand(uint32_t cardId, int discardPileSize) {
    _leftHand.removeCardId(cardId);
    takeCard(cardId);
    _discardPile.setCount(discardPileSize);
    reorganizeHand(_leftHand);
}

void BattleUI::discardFromRightHand(uint32_t cardId, int discardPileSize) {
    _rightHand.removeCardId(cardId);
    takeCard(cardId);
    _discardPile.setCount(discardPileSize);
    reorganizeHand(_rightHand);
}

#pragma endregion

#pragma region Helpers

CardUI BattleUI::takeCard(uint32_t cardId) {
    auto it = std::find_if(_cards.begin(), _cards.end(), [cardId](const CardUI& card) {
        return card.id() == cardId;
    });
    if (it == _cards.end()) {
        TraceLog(LOG_ERROR, "BattleUI: takeCard %u not found in card list", cardId);
        return CardUI(cardId); // shouldn't happen; indicates a logic error upstream
    }

    CardUI card = *it; // transform is parentless here, so the copy is safe
    _cards.erase(it);
    return card;
}

void BattleUI::addCard(uint32_t cardId, std::string texture) {
    CardUI c(cardId);
    c.transform = _drawPile.getAnchorPoint().transform.asWorldTransform();
    c.sprite.texture = texture;
    c.hitbox.setRectangle(c.sprite.size);
    _cards.push_back(c);
}

void BattleUI::reorganizeHand(const FixedSlotHandUI& hand) {
    size_t slot = 0;
    for (uint32_t id : hand.getCardIds()) {
        if (slot >= hand.slotCount()) {
            TraceLog(
                LOG_WARNING,
                "BattleUI: hand holds %d cards but only %d slots; %d not placed",
                (int)hand.getCardIds().size(),
                (int)hand.slotCount(),
                (int)hand.getCardIds().size() - (int)hand.slotCount()
            );
            break;
        }

        auto it = std::find_if(_cards.begin(), _cards.end(), [id](const CardUI& card) {
            return card.id() == id;
        });
        if (it != _cards.end()) {
            const TransformComponent& target = hand.getPosition(slot).transform;
            it->transform.position = target.position;
            it->transform.rotation = target.rotation;
            it->transform.scale = target.scale;
        } else {
            TraceLog(
                LOG_WARNING, "BattleUI: card %u is in a hand but missing from the card list", id
            );
        }
        ++slot;
    }
}

#pragma endregion
