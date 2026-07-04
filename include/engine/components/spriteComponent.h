#pragma once

#include "raylib.h"
#include <string>

struct SpriteComponent {
    std::string texture;
    Vector2 size = {80.0f, 110.0f}; // on-screen size in pixels (texture is stretched to fit)
    Color color = WHITE;
    int layer = 0;
    bool visible = true;
};
