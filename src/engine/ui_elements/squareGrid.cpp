#include "engine/ui_elements/squareGrid.h"
#include <cmath>

SquareGrid::SquareGrid(const Vector2 dimensions, const Vector2 offset)
    : _dimensions(dimensions),
      _offset(offset) {
}

double SquareGrid::distance(const Vector2& from, const Vector2& to) const {
    return std::sqrt(std::pow((from.x - to.x), 2) + std::pow((from.y - to.y), 2));
}

TransformComponent SquareGrid::pointTransform(int col, int row) const {
    Vector2 local = {static_cast<float>(col) * _offset.x, static_cast<float>(row) * _offset.y};
    float r = _transform.worldRotation();
    float s = _transform.worldScale();
    Vector2 pos = _transform.worldPosition();
    float c = cosf(r), sn = sinf(r);
    Vector2 scaled = {local.x * s, local.y * s};
    Vector2 rotated = {scaled.x * c - scaled.y * sn, scaled.x * sn + scaled.y * c};

    TransformComponent t;
    t.position = {pos.x + rotated.x, pos.y + rotated.y};
    t.rotation = r;
    t.scale = s;
    return t;
}

const std::vector<Point2>& SquareGrid::getPoints() const {
    return _points;
}

void SquareGrid::updatePoints(const TransformComponent& ownerTransform) {
    _transform = ownerTransform;
    int columns = static_cast<int>(_dimensions.x);
    int rows = static_cast<int>(_dimensions.y);
    _points.clear();
    _points.reserve(static_cast<size_t>(columns) * static_cast<size_t>(rows));
    for (int i = 0; i < columns; i++) {
        for (int j = 0; j < rows; j++) {
            _points.emplace_back(pointTransform(i, j));
        }
    }
}
