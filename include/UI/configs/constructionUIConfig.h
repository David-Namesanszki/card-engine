#pragma once

#include "raylib.h"
#include <string>

struct ConstructionUIConfig {
    Vector2 spriteSize = {187.0f, 135.0f};
    Vector2 pipSize = {25.0f, 25.0f};

    int textFontSize = 16;
    Color textColor = {245, 235, 210, 255};
    Color textOutlineColor = {25, 25, 30, 255};
    int textOutlineThickness = 1;

    std::string durabilityPipTexture = "assets/pips/durability_pip.png";

    std::string namePipTexture = "assets/pips/name_pip.png";
    Vector2 namePipSize = {56.0f, 16.0f};
    Vector2 namePipOffset = {0.0f, -40.0f};
    int nameFontSize = 10;

    Vector2 durabilityPipOffset = {-36.0f, -6.0f};
    Vector2 durabilityTextOffset = {-41.0f, -14.0f};
};
