#pragma once

#include "raylib.h"
#include "engine/transformComponent.h"
#include <vector>

struct HitBoxComponent {
    Vector2 position;            // offset in the entity's local space
    std::vector<Vector2> points; // polygon vertices, relative to `position`
    int layer;

    // Tests `point` (world space) against the polygon, accounting for the
    // entity's world position, rotation and scale supplied by `xf`.
    bool isInside(const Vector2& point, const TransformComponent& xf) const;
};
