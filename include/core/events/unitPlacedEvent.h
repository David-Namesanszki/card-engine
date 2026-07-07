#pragma once

#include <cstdint>
#include "engine/hexCoord.h"

struct UnitPlacedEvent {
    uint32_t unitId;
    HexCoord at;
};
