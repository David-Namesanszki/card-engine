#pragma once
#include <array>
#include <cstddef>
#include <cstdlib>
#include <functional>

// Axial hex coordinates and the math the game rules need.
// Deliberately raylib-free so the core can depend on it; the hex<->pixel
// conversions live in engine/components/hexComponent.h.

struct HexCoord {
    int q, r;
    bool operator==(const HexCoord& other) const {
        return q == other.q && r == other.r;
    }
    HexCoord operator+(const HexCoord& other) const {
        return {q + other.q, r + other.r};
    }
    HexCoord operator-() const {
        return {-q, -r};
    }
};

struct HexCoordHash {
    size_t operator()(const HexCoord& h) const {
        size_t hq = std::hash<int>{}(h.q);
        size_t hr = std::hash<int>{}(h.r);
        return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
    }
};

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
