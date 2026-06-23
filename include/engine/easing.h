#pragma once
#include <cmath>

namespace Easing {
    inline float linear(float t)    { return t; }
    inline float easeIn(float t)    { return t * t * t; }
    inline float easeOut(float t)   { return 1.0f - std::pow(1.0f - t, 3.0f); }
    inline float easeInOut(float t) {
        return t < 0.5f ? 4*t*t*t : 1.0f - std::pow(-2.0f*t + 2.0f, 3.0f) / 2.0f;
    }
}
