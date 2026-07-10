#pragma once

#include "raylib.h"
#include <string>

struct TextComponent {
    std::string text;
    int fontSize = 12; // on-screen size in pixels (texture is stretched to fit)
    Color color = WHITE;
    int outlineThickness = 0; // pixels; 0 disables the outline
    Color outlineColor = BLACK;
    int layer = 0;
    bool visible = true;
};
