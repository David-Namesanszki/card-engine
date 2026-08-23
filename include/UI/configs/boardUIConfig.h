#pragma once

#include "engine/components/transformComponent.h"
#include "engine/components/spriteComponent.h"
#include "engine/components/hitBoxComponent.h"

// Pure theme: which tiles exist is game content and reaches BoardUI as a
// constructor parameter sourced from the core board.
struct BoardUIConfig {
    TransformComponent transform;

    // Hex radius in pixels; spreads the grid and is the natural knob for
    // sizing the board.
    ;

    // Where a unit stands on its tile, local to the tile's centre.
    Vector2 unitAnchorOffset = {0.0f, -20.0f};
};
