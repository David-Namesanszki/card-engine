#pragma once

#include <cstdint>
#include "engine/hexCoord.h"

struct UnitDiedEvent {
    uint32_t unitId;
    HexCoord at;
};
