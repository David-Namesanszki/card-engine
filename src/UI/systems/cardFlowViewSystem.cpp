#include "UI/systems/cardFlowViewSystem.h"
#include <algorithm>
#include "raylib.h"

CardFlowViewSystem::CardFlowViewSystem(
    HandUIConfig leftHandUI,
    HandUIConfig rightHandUI,
    CardPileUIConfig drawPileUI,
    CardPileUIConfig discardPileUI,
    size_t drawPileSize,
    size_t discardPileSize
)
    : _leftHandUI(leftHandUI),
      _rightHandUI(rightHandUI),
      _drawPile(drawPileUI, drawPileSize),
      _discardPile(discardPileUI, discardPileSize) {
}

void CardFlowViewSystem::update(float dt) {
    // animations
}

void CardFlowViewSystem::render(RenderSystem& renderer) {
    renderer.renderCardPile(_drawPile);
    renderer.renderCardPile(_discardPile);
    renderer.renderHand(_leftHandUI);
    renderer.renderHand(_rightHandUI);

    for (const auto& cardUI : _cardUIs)
        renderer.renderCard(cardUI);
}

const std::list<CardUI>& CardFlowViewSystem::getCards() const {
    return _cardUIs;
}

void CardFlowViewSystem::refillDrawPile(size_t drawPileSize, size_t discardPileSize) {
    _drawPile.setCount(drawPileSize);
    _discardPile.setCount(discardPileSize);
}

void CardFlowViewSystem::drawCard(uint32_t cardId, std::string splashArt, size_t drawPileSize) {
    CardUI& card = _cardUIs.emplace_back(cardId, std::move(splashArt));
    card.transform = _drawPile.getAnchorPoint().transform.asWorldTransform();

    _drawPile.setCount(drawPileSize);
    addCardToLeftHand(cardId, 0);
    reorganizeLeftHand();
}

void CardFlowViewSystem::transferCard(uint32_t cardId) {
    removeCardFromLeftHand(cardId);
    addCardToRightHand(cardId, 0);
    reorganizeLeftHand();
    reorganizeRightHand();
}

void CardFlowViewSystem::discardCard(HandType type, uint32_t cardId, size_t discardPileSize) {
    if (type == HandType::Left) {
        removeCardFromLeftHand(cardId);
        reorganizeLeftHand();
    } else {
        removeCardFromRightHand(cardId);
        reorganizeRightHand();
    }

    _discardPile.setCount(discardPileSize);
    _cardUIs.erase(std::find_if(_cardUIs.begin(), _cardUIs.end(), [cardId](const CardUI& card) {
        return card.id() == cardId;
    }));
}

void CardFlowViewSystem::addCardToLeftHand(uint32_t cardId, size_t index) {
    _leftHand.insert(std::next(_leftHand.begin(), index), cardId);
}

void CardFlowViewSystem::addCardToRightHand(uint32_t cardId, size_t index) {
    _rightHand.insert(std::next(_rightHand.begin(), index), cardId);
}

void CardFlowViewSystem::removeCardFromLeftHand(uint32_t cardId) {
    auto it = std::find(_leftHand.begin(), _leftHand.end(), cardId);
    if (it != _leftHand.end())
        _leftHand.erase(it);
}

void CardFlowViewSystem::removeCardFromRightHand(uint32_t cardId) {
    auto it = std::find(_rightHand.begin(), _rightHand.end(), cardId);
    if (it != _rightHand.end())
        _rightHand.erase(it);
}

void CardFlowViewSystem::reorganizeLeftHand() {
    size_t slot = 0;
    for (uint32_t id : _leftHand) {
        if (slot >= _leftHandUI.slotCount()) {
            TraceLog(
                LOG_WARNING,
                "BattleScene: hand holds %d cards but only %d slots; %d not placed",
                (int)_leftHand.size(),
                (int)_leftHandUI.slotCount(),
                (int)_leftHand.size() - (int)_leftHandUI.slotCount()
            );
            break;
        }

        auto it = std::find_if(_cardUIs.begin(), _cardUIs.end(), [id](const CardUI& card) {
            return card.id() == id;
        });
        if (it != _cardUIs.end()) {
            const TransformComponent& target = _leftHandUI.getPosition(slot).transform;
            it->transform.position = target.position;
            it->transform.rotation = target.rotation;
            it->transform.scale = target.scale;
        } else {
            TraceLog(
                LOG_WARNING, "BattleScene: card %u is in a hand but missing from the card list", id
            );
        }
        ++slot;
    }
}

void CardFlowViewSystem::reorganizeRightHand() {
    size_t slot = 0;
    for (uint32_t id : _rightHand) {
        if (slot >= _rightHandUI.slotCount()) {
            TraceLog(
                LOG_WARNING,
                "BattleScene: hand holds %d cards but only %d slots; %d not placed",
                (int)_rightHand.size(),
                (int)_rightHandUI.slotCount(),
                (int)_rightHand.size() - (int)_rightHandUI.slotCount()
            );
            break;
        }

        auto it = std::find_if(_cardUIs.begin(), _cardUIs.end(), [id](const CardUI& card) {
            return card.id() == id;
        });
        if (it != _cardUIs.end()) {
            const TransformComponent& target = _rightHandUI.getPosition(slot).transform;
            it->transform.position = target.position;
            it->transform.rotation = target.rotation;
            it->transform.scale = target.scale;
        } else {
            TraceLog(
                LOG_WARNING, "BattleScene: card %u is in a hand but missing from the card list", id
            );
        }
        ++slot;
    }
}