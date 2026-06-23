#pragma once 

#include "raylib.h"
#include <vector>

struct HitBoxObject {
    Vector2 position = {0, 0};
    Vector2 size = {80, 110};
    float rotation = 0.0f;
    int layer;

    HitBoxObject* parent   = nullptr;
    std::vector<HitBoxObject*> children;

    void addObject(const HitBoxObject* object);
};