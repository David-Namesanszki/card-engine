#pragma once

#include <vector>
#include <raylib.h>
#include "engine/components/transformComponent.h"
#include "engine/ui_elements/point2.h"

// Grid shape descriptor. dimensions/offset define the grid's shape; it has no
// position of its own -- the owning UI element calls updatePoints(transform)
// with its own transform whenever it moves, which places and caches the points.
class SquareGrid {
  public:
    SquareGrid(const Vector2 dimensions, const Vector2 offset);
    double distance(const Vector2& from, const Vector2& to) const;
    const std::vector<Point2>& getPoints() const;
    void updatePoints(const TransformComponent& ownerTransform);

  private:
    // Local (col*offset.x, row*offset.y) offset, scaled and rotated through the
    // grid's world rotation/scale, then translated to its world position --
    // same convention as TransformComponent::worldPosition() and QuadraticBezier.
    TransformComponent pointTransform(int col, int row) const;

    TransformComponent _transform;
    std::vector<Point2> _points;
    Vector2 _dimensions;
    Vector2 _offset;
};
