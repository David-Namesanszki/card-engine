#pragma once

#include <cstdint>
#include "core/types/handType.h"

struct CardDiscardedEvent {
    uint32_t cardId;
    HandType hand;
};
