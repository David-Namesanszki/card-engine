#pragma once

#include "raylib.h"
#include <vector>
#include <cmath>

struct TransformComponent {
    Vector2 position = {0, 0};
    float rotation = 0.0f;
    float scale = 1.0f;

    TransformComponent* parent = nullptr;
    std::vector<TransformComponent*> children;

    float worldRotation() const {
        return parent ? parent->worldRotation() + rotation : rotation;
    }

    float worldScale() const {
        return parent ? parent->worldScale() * scale : scale;
    }

    Vector2 worldPosition() const {
        if (!parent)
            return position;
        float pr = parent->worldRotation();
        float ps = parent->worldScale();
        Vector2 pw = parent->worldPosition();
        // local offset, scaled then rotated into parent's frame, then translated
        float c = cosf(pr), s = sinf(pr);
        Vector2 scaled = {position.x * ps, position.y * ps};
        Vector2 rotated = {scaled.x * c - scaled.y * s, scaled.x * s + scaled.y * c};
        return {pw.x + rotated.x, pw.y + rotated.y};
    }

    void addChild(TransformComponent* child);
    void removeChild(TransformComponent* child);

    ~TransformComponent();
};