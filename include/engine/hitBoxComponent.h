#pragma once

#include "raylib.h"

struct HitBoxComponent {
    Vector2 position = {0, 0};
    Vector2 size = {80, 110};
    float rotation = 0.0f;
    int layer = 0;
};
