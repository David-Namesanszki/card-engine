#pragma once

#include <cstdint>
#include "engine/hexCoord.h"

struct BoardPiecePlacedEvent {
    uint32_t id;
    HexCoord coord;
};