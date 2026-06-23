#pragma once

#include "engine/renderObject.h"
#include "UI/rightHandUI.h"
#include "UI/leftHandUI.h"
#include "UI/discardPileUI.h"
#include "UI/drawPileUI.h"
#include "UI/configs/battleUIConfig.h"

class BattleUI {
public:
    BattleUI(
        const BattleUIConfig& config,
        const LeftHandUIConfig& leftHandConfig,
        const RightHandUIConfig& rightHandConfig,
        const DiscardPileUIConfig& discardPileConfig,
        const DrawPileUIConfig& drawPileConfig);
    void drawCard(uint32_t card);
    void discardFromLeftHand(uint32_t card);
    void discardFromRightHand(uint32_t card);

private:
    RenderObject _ro;

    BattleUIConfig _config;
    DiscardPileUI _discardPile;
    DrawPileUI _drawPile;
    LeftHandUI _leftHand;
    RightHandUI _rightHand;

    Animator& _animator;
};