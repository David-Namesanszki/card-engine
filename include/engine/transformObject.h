#pragma once

#include "raylib.h"
#include <vector>

struct TransformObject {
    Vector2 position = {0, 0};
    Vector2 size = {80, 110};
    float rotation = 0.0f;
    float scale = 1.0f;

    TransformObject* parent   = nullptr;
    std::vector<TransformObject*> children;

    void addObject(const TransformObject* object);
};