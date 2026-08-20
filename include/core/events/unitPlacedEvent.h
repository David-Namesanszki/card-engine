#pragma once

#include <cstdint>
#include "engine/hexCoord.h"
#include "core/entities/boardPieces/unit.h"

struct UnitPlacedEvent {
    uint32_t unitId;
    HexCoord at;
    // Snapshot of the placed unit, so subscribers don't have to reach back
    // into the battle for its stats.
    Unit unit;
};
