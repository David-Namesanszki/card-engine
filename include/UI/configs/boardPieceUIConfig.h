#pragma once

#include "raylib.h"
#include <string>

// Presentation shared by everything that stands on a tile: the portrait, the
// hitbox that matches it, and the name banner above it. Per-type readouts live
// in the config that embeds this one.
struct BoardPieceUIConfig {
    Vector2 spriteSize = {187.0f, 135.0f};
    Vector2 pipSize = {25.0f, 25.0f};

    int textFontSize = 16;
    Color textColor = {245, 235, 210, 255}; // cream, like the mockup numbers
    Color textOutlineColor = {25, 25, 30, 255};
    int textOutlineThickness = 1;

    // Name banner above the sprite; the text centres itself on the banner.
    std::string namePipTexture = "assets/pips/name_pip.png";
    Vector2 namePipSize = {56.0f, 16.0f};
    Vector2 namePipOffset = {0.0f, -40.0f};
    int nameFontSize = 10;
};
