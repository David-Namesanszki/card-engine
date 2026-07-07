#pragma once
#include <cmath>
#include "raylib.h"
#include "engine/hexCoord.h"

// Presentation-side hex<->pixel conversions. HexCoord itself and the hex math
// (neighbors, distance) live in engine/hexCoord.h so the core can use them
// without pulling in raylib.

enum class HexOrientation {
    FlatTop,
    PointyTop
};

inline Vector2 hexToPixelFlat(HexCoord h, float size) {
    return {size * (3.0f / 2.0f) * h.q, size * (sqrtf(3.0f) / 2.0f * h.q + sqrtf(3.0f) * h.r)};
}

inline Vector2 hexToPixelPointy(HexCoord h, float size) {
    return {size * (sqrtf(3.0f) * h.q + sqrtf(3.0f) / 2.0f * h.r), size * (3.0f / 2.0f) * h.r};
}

inline HexCoord pixelToHexFlat(Vector2 p, float size) {
    float fq = (2.0f / 3.0f * p.x) / size;
    float fr = (-1.0f / 3.0f * p.x + sqrtf(3.0f) / 3.0f * p.y) / size;
    float fs = -fq - fr;

    int q = (int)roundf(fq);
    int r = (int)roundf(fr);
    int s = (int)roundf(fs);

    float dq = fabsf(q - fq);
    float dr = fabsf(r - fr);
    float ds = fabsf(s - fs);

    if (dq > dr && dq > ds)
        q = -r - s;
    else if (dr > ds)
        r = -q - s;

    return {q, r};
}

inline HexCoord pixelToHexPointy(Vector2 p, float size) {
    float fq = (sqrtf(3.0f) / 3.0f * p.x - 1.0f / 3.0f * p.y) / size;
    float fr = (2.0f / 3.0f * p.y) / size;
    float fs = -fq - fr;

    int q = (int)roundf(fq);
    int r = (int)roundf(fr);
    int s = (int)roundf(fs);

    float dq = fabsf(q - fq);
    float dr = fabsf(r - fr);
    float ds = fabsf(s - fs);

    if (dq > dr && dq > ds)
        q = -r - s;
    else if (dr > ds)
        r = -q - s;

    return {q, r};
}
