#pragma once

#include "engine/components/transformComponent.h"
#include "engine/components/spriteComponent.h"
#include "raylib.h"
#include <string>

struct ActionPointsPanelUIConfig {
    TransformComponent transform;
    SpriteComponent background;

    int maxActionPoints = 3;

    // Straight-line layout (unlike FixedSlotHandUI's arc): pips are evenly
    // spaced between these two offsets, no curve/rotation involved.
    Vector2 lineStartOffset = {-60.0f, 0.0f};
    Vector2 lineEndOffset = {60.0f, 0.0f};
    Vector2 pipSize = {24.0f, 24.0f};

    std::string availableTexture = "assets/action_point.png";
    std::string spentTexture = "assets/action_point_spent.png";
};
