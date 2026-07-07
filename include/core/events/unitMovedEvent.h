#pragma once

#include <cstdint>
#include "engine/hexCoord.h"

struct UnitMovedEvent {
    uint32_t unitId;
    HexCoord from;
    HexCoord to;
};
