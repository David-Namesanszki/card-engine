#include "engine/hitBoxComponent.h"
#include <cmath>

namespace {

// Standard ray-casting (even-odd) point-in-polygon test.
// `poly` and `p` are expressed in the same (local) space.
bool pointInPolygon(const Vector2& p, const std::vector<Vector2>& poly) {
    bool inside = false;
    size_t n = poly.size();
    for (size_t i = 0, j = n - 1; i < n; j = i++) {
        const Vector2& a = poly[i];
        const Vector2& b = poly[j];
        bool straddles = (a.y > p.y) != (b.y > p.y);
        if (straddles) {
            float xCross = a.x + (p.y - a.y) / (b.y - a.y) * (b.x - a.x);
            if (p.x < xCross)
                inside = !inside;
        }
    }
    return inside;
}

} // namespace

bool HitBoxComponent::isInside(const Vector2& point, const TransformComponent& xf) const {
    if (points.size() < 3)
        return false;

    // 1. world -> entity-local: undo the entity's *world* transform so that
    //    parented/nested rotation and scale are respected.
    Vector2 world = xf.worldPosition();
    Vector2 local = {point.x - world.x, point.y - world.y};

    // Rotate by -worldRotation (radians, matching TransformComponent).
    float rad = -xf.worldRotation();
    float c = std::cos(rad);
    float s = std::sin(rad);
    Vector2 rotated = {local.x * c - local.y * s, local.x * s + local.y * c};

    // Undo scale (guard against a zero/negative scale).
    float scale = xf.worldScale();
    if (scale != 0.0f) {
        rotated.x /= scale;
        rotated.y /= scale;
    }

    // 2. Subtract the hitbox's own local offset so we are in polygon space.
    Vector2 query = {rotated.x - position.x, rotated.y - position.y};

    // 3. Point-in-polygon against the static local `points`.
    return pointInPolygon(query, points);
}
