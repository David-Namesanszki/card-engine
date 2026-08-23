#pragma once

#include "raylib.h"
#include "UI/configs/boardPieceUIConfig.h"
#include <string>

struct ConstructionUIConfig {
    BoardPieceUIConfig piece;

    std::string durabilityPipTexture = "assets/pips/durability_pip.png";
    Vector2 durabilityPipOffset = {-36.0f, -6.0f};
    Vector2 durabilityTextOffset = {-41.0f, -14.0f};
};
