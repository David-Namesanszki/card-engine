#pragma once

#include "raylib.h"
#include <string>

struct SpriteComponent {
    std::string texture;
    Color color = WHITE;
    int layer = 0;
    bool visible = true;
};
