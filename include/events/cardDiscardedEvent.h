#pragma once

#include <string>

struct CardDiscardedEvent {
    uint32_t cardId;
    std::string cardDiscardedSound;
    std::string cardDiscardedAnimation;
};