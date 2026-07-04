#pragma once

#include <string>
#include "core/types/handType.h"

struct CardDiscardedEvent {
    uint32_t cardId;
    HandType hand;
    std::string cardDiscardedSound;
    std::string cardDiscardedAnimation;
};