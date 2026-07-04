#pragma once
#include <array>
#include <cmath>
#include <cstddef>
#include <functional>
#include "raylib.h"

enum class HexOrientation {
    FlatTop,
    PointyTop
};

struct HexCoord {
    int q, r;
    bool operator==(const HexCoord& other) const {
        return q == other.q && r == other.r;
    }
};

struct HexCoordHash {
    size_t operator()(const HexCoord& h) const {
        size_t hq = std::hash<int>{}(h.q);
        size_t hr = std::hash<int>{}(h.r);
        return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
    }
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

inline std::array<HexCoord, 6> hexNeighbors(HexCoord h) {
    return {
        {{h.q + 1, h.r - 1},
         {h.q + 1, h.r},
         {h.q, h.r + 1},
         {h.q - 1, h.r + 1},
         {h.q - 1, h.r},
         {h.q, h.r - 1}}
    };
}

inline int hexDistance(HexCoord a, HexCoord b) {
    int dq = a.q - b.q;
    int dr = a.r - b.r;
    int ds = -dq - dr;
    return (abs(dq) + abs(dr) + abs(ds)) / 2;
}
