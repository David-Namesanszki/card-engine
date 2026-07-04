#pragma once

#include "raylib.h"
#include "engine/components/transformComponent.h"
#include "engine/components/hexComponent.h" // HexOrientation
#include <vector>
#include <cmath>

struct HitBoxComponent {
    Vector2 position = {0.0f, 0.0f}; // offset in the entity's local space
    std::vector<Vector2> points;     // polygon vertices, relative to `position`
    int layer = 0;

    // Configures the hitbox as a rectangle of `size`, centred on the entity
    // origin. This matches the renderer, which draws a sprite centred on its
    // transform at sprite.size (scale is applied/undone on both sides).
    void setRectangle(Vector2 size) {
        float hw = size.x * 0.5f;
        float hh = size.y * 0.5f;
        position = {0.0f, 0.0f};
        points = {{-hw, -hh}, {hw, -hh}, {hw, hh}, {-hw, hh}};
    }

    // Configures the hitbox as a regular hexagon of `radius`, centred on the
    // entity origin. Pass the same HexOrientation used to position the entity
    // (hexToPixelFlat/hexToPixelPointy in hexComponent.h), or the hitbox won't
    // line up with what's actually drawn.
    void setHexagon(float radius, HexOrientation orientation = HexOrientation::FlatTop) {
        position = {0.0f, 0.0f};
        points.clear();
        points.reserve(6);
        float startDeg = (orientation == HexOrientation::PointyTop) ? -30.0f : 0.0f;
        for (int i = 0; i < 6; ++i) {
            float angle = (startDeg + 60.0f * i) * DEG2RAD;
            points.push_back({radius * cosf(angle), radius * sinf(angle)});
        }
    }
};
