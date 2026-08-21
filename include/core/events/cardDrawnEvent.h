#pragma once

#include <cstdint>

struct CardDrawnEvent {
    Card card;
    size_t drawPileSize;
};
