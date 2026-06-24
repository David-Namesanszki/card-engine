#pragma once

#include "raylib.h"
#include <vector>
#include <algorithm>

struct TransformComponent {
    Vector2 position = {0, 0};
    Vector2 size = {80, 110};
    float rotation = 0.0f;
    float scale = 1.0f;

    TransformComponent* parent = nullptr;
    std::vector<TransformComponent*> children;

    Vector2 worldPosition() const {
        if (!parent) return position;
        Vector2 p = parent->worldPosition();
        return {p.x + position.x, p.y + position.y};
    }

    void addChild(TransformComponent* child) {
        children.push_back(child);
        child->parent = this;
    }

    void removeChild(TransformComponent* child) {
        auto it = std::find(children.begin(), children.end(), child);
        if (it != children.end()) children.erase(it);
        child->parent = nullptr;
    }
};