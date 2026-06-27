#pragma once

#include "engine/easing.h"
#include "engine/animator.h"
#include "UI/anchorPoint.h"
#include "UI/cardUI.h"
#include "raylib.h"
#include <functional>

using AnimDef = std::function<Animation(TransformComponent&, const AnchorPoint&)>;

struct HandUIConfig {
    TransformComponent transform = {0.0f, 0.0f};
    SpriteComponent sprite;

    // Arc layout
    int slotCount = 5;
    Vector2 arcStartOffset = {-200.0f, 0.0f};
    Vector2 arcEndOffset = {200.0f, 0.0f};
    Vector2 controlOffset = {0.0f, -60.0f};
};
