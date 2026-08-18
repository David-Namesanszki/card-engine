#pragma once

#include "engine/components/transformComponent.h"

class Point2 {
  public:
    Point2(const TransformComponent transform)
        : transform(transform) {
    }

    TransformComponent transform;
};