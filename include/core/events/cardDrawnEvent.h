#pragma once

#include <string>

struct CardDrawnEvent {
    uint32_t cardId;
    std::string drawSound;
    std::string drawAnimation;
};