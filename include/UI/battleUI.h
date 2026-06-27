#pragma once

#include "engine/animator.h"
#include "UI/hands/fixedSlotHandUI.h"
#include "UI/cardPileUI.h"
#include "UI/configs/battleUIConfig.h"
#include <list>
#include "UI/cardUI.h"

class BattleUI {
  public:
    BattleUI(
        BattleUIConfig config = {},
        HandUIConfig leftHandConfig = {},
        HandUIConfig rightHandConfig = {},
        CardPileUIConfig discardPileConfig = {},
        CardPileUIConfig drawPileConfig = {}
    );

    TransformComponent transform;
    SpriteComponent sprite;

    void drawCard(uint32_t cardId);
    void discardFromLeftHand(uint32_t cardId);
    void discardFromRightHand(uint32_t cardId);

    const CardPileUI& drawPile() const;
    const CardPileUI& discardPile() const;
    const FixedSlotHandUI& leftHand() const;
    const FixedSlotHandUI& rightHand() const;
    const std::list<CardUI>& cards() const;

  private:
    BattleUIConfig _config;

    CardPileUI _drawPile;
    CardPileUI _discardPile;
    FixedSlotHandUI _leftHand;
    FixedSlotHandUI _rightHand;
    std::list<CardUI> _cards;
};
