#include "engine/ui_elements/bezierCurve.h"
#include <cmath>

QuadraticBezier::QuadraticBezier(
    Vector2 startOffset,
    Vector2 controlOffset,
    Vector2 endOffset,
    int pointCount
)
    : _startOffset(startOffset),
      _controlOffset(controlOffset),
      _endOffset(endOffset),
      _pointCount(pointCount) {
}

const std::vector<Vector2>& QuadraticBezier::getPoints() const {
    return _points;
}

void QuadraticBezier::updatePoints(const TransformComponent& ownerTransform) {
    _transform = ownerTransform;
    rebuildPoints();
}

Vector2 QuadraticBezier::applyTransform(Vector2 offset) const {
    float r = _transform.worldRotation();
    float s = _transform.worldScale();
    Vector2 pos = _transform.worldPosition();
    float c = cosf(r), sn = sinf(r);
    Vector2 scaled = {offset.x * s, offset.y * s};
    Vector2 rotated = {scaled.x * c - scaled.y * sn, scaled.x * sn + scaled.y * c};
    return {pos.x + rotated.x, pos.y + rotated.y};
}

Vector2 QuadraticBezier::pointAt(float t) const {
    Vector2 p0 = applyTransform(_startOffset);
    Vector2 p1 = applyTransform(_controlOffset);
    Vector2 p2 = applyTransform(_endOffset);
    float u = 1.0f - t;
    return {
        u * u * p0.x + 2.0f * u * t * p1.x + t * t * p2.x,
        u * u * p0.y + 2.0f * u * t * p1.y + t * t * p2.y
    };
}

float QuadraticBezier::rotation(float t) const {
    Vector2 p0 = applyTransform(_startOffset);
    Vector2 p1 = applyTransform(_controlOffset);
    Vector2 p2 = applyTransform(_endOffset);
    float u = 1.0f - t;
    float dx = 2.0f * u * (p1.x - p0.x) + 2.0f * t * (p2.x - p1.x);
    float dy = 2.0f * u * (p1.y - p0.y) + 2.0f * t * (p2.y - p1.y);
    return atan2f(dy, dx);
}

void QuadraticBezier::rebuildPoints() {
    _points.clear();
    if (_pointCount <= 1) {
        _points.push_back(pointAt(0.0f));
        return;
    }
    _points.reserve(_pointCount);
    for (int i = 0; i < _pointCount; i++) {
        float t = static_cast<float>(i) / static_cast<float>(_pointCount - 1);
        _points.push_back(pointAt(t));
    }
}
