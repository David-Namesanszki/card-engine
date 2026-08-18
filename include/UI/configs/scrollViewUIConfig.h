#pragma once

#include "raylib.h"
#include "engine/components/transformComponent.h"
#include "engine/components/spriteComponent.h"

enum class ScrollDirection {
    Horizontal,
    Vertical
};

// Pure theme for a scrollable viewport. What scrolls is added by the owning
// widget (see ScrollViewUI); this only describes the clip/offset geometry.
struct ScrollViewUIConfig {
    TransformComponent transform; // viewport centre
    SpriteComponent background;

    Vector2 viewportSize = {768.0f, 216.0f};
    ScrollDirection direction = ScrollDirection::Horizontal;
    float scrollStep = 60.0f; // pixels per wheel notch
};
