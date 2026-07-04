#pragma once

#include <cstdint>
#include <vector>

struct CardPlayedEvent {
    uint32_t cardId;
    std::vector<uint32_t> targets;
};