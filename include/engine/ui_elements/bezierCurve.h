#pragma once

#include "raylib.h"
#include "engine/components/transformComponent.h"
#include <vector>

// Quadratic bezier shape descriptor. startOffset/controlOffset/endOffset define
// the curve's shape; it has no position of its own -- the owning UI element
// calls updatePoints(transform) with its own transform whenever it moves,
// which places and caches the sampled points. Owns its sampled points the same
// way SquareGrid owns its point list -- computed by updatePoints and read back
// via getPoints(), rather than recomputed by every caller.
class QuadraticBezier {
  public:
    QuadraticBezier(
        Vector2 startOffset,
        Vector2 controlOffset,
        Vector2 endOffset,
        int pointCount
    );

    const std::vector<Vector2>& getPoints() const;
    void updatePoints(const TransformComponent& ownerTransform);

    // Tangent direction along the curve at t (0..1), in radians, using the
    // transform from the last updatePoints() call.
    float rotation(float t) const;

  private:
    Vector2 applyTransform(Vector2 offset) const;
    Vector2 pointAt(float t) const;
    void rebuildPoints();

    TransformComponent _transform;
    Vector2 _startOffset;
    Vector2 _controlOffset;
    Vector2 _endOffset;
    int _pointCount;
    std::vector<Vector2> _points;
};
