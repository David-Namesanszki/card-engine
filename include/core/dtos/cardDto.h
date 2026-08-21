#pragma once

#include <string>
#include "core/types/cardType.h"

struct CardDto {
    std::string name;
    CardType type;
    std::string description;
    std::string splashArt;
    int fireCost;
};