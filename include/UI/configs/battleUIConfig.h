#pragma once

#include "engine/animator.h"
#include "UI/anchorPoint.h"
#include "UI/cardUI.h"
#include <functional>

using AnimFactory = std::function<Animation(CardUI&, const AnchorPoint&)>;

struct BattleUIConfig {
    // RenderObject visual properties
    Vector2 position = {0.0f,   0.0f};
    Vector2 size     = {400.0f, 150.0f};
    Color   color    = {30, 30, 50, 200};
    int     layer    = 0;

    // Animations
    AnimFactory drawAnimDef = nullptr;
    AnimFactory discardAnimDef = nullptr;
};