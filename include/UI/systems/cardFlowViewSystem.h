#pragma once

#include <cstdint>
#include <list>
#include "UI/cardUI.h"
#include "UI/fixedSlotHandUI.h"
#include "core/types/handType.h"
#include "UI/cardPileUI.h"

#include "engine/systems/renderSystem.h"

class CardFlowViewSystem {
  public:
    CardFlowViewSystem(
        HandUIConfig leftHandUI,
        HandUIConfig rightHandUI,
        CardPileUIConfig drawPileUI,
        CardPileUIConfig discardPileUI,
        size_t drawPileSize,
        size_t discardPileSize
    );

    void update(float dt);
    void render(RenderSystem& renderer);

    const std::list<CardUI>& getCards() const;

    void discardCard(HandType type, uint32_t cardId, size_t discardPileSize);
    void transferCard(uint32_t cardId);
    void drawCard(uint32_t cardId, std::string splashArt, size_t drawPileSize);
    void refillDrawPile(size_t drawPileSize, size_t discardPileSize);

  private:
    void reorganizeLeftHand();
    void reorganizeRightHand();
    void addCardToLeftHand(uint32_t cardId, size_t index);
    void addCardToRightHand(uint32_t cardId, size_t index);
    void removeCardFromLeftHand(uint32_t cardId);
    void removeCardFromRightHand(uint32_t cardId);

    FixedSlotHandUI _leftHandUI;
    FixedSlotHandUI _rightHandUI;
    CardPileUI _drawPile;
    CardPileUI _discardPile;
    std::list<CardUI> _cardUIs;

    std::list<uint32_t> _leftHand;
    std::list<uint32_t> _rightHand;
};