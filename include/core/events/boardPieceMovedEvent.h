#pragma once

#include <cstdint>
#include "engine/hexCoord.h"

struct BoardPieceMovedEvent {
    uint32_t id;
    HexCoord from;
    HexCoord to;
};