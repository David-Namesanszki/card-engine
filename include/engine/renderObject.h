#pragma once

#include "raylib.h"
#include <string>

struct RenderObject {
    std::string texture;
    std::string text;
    Vector2     position = {0, 0};
    Vector2     size     = {80, 110};
    float       rotation = 0.0f;
    float       scale    = 1.0f;
    Color       color    = WHITE;
    int         layer    = 0;
    bool        visible  = true;
};
