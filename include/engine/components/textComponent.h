#pragma once

#include "raylib.h"
#include <string>

struct TextComponent {
    std::string text;
    int fontSize = 12; // on-screen size in pixels (texture is stretched to fit)
    Color color = WHITE;
    int layer = 0;
    bool visible = true;
};
