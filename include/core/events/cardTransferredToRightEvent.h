#pragma once

#include <string>

struct CardTransferredToRightEvent {
    uint32_t cardId;
    std::string cardTransferredToRightAnimation;
    std::string cardTransferredToRightSound;
};
