#pragma once

#include "raylib.h"
#include "UI/configs/scrollViewUIConfig.h"

// Pure theme for a grid of uniformly-sized, uniformly-spaced items that
// wraps into a new line after wrapCount items and scrolls along whichever
// axis keeps growing:
//   wrapCount == 0  -> a single row, unbounded, scrolling horizontally
//                      (a plain list — e.g. the roster strip)
//   wrapCount >= 1  -> wrapCount columns, unbounded rows, scrolling
//                      vertically (a card-catalogue-style grid;
//                      wrapCount == 1 degenerates to a vertical list)
// scrollView.direction is derived from wrapCount and overwritten by
// ScrollableGridUI — whatever is set here is ignored.
struct ScrollableGridUIConfig {
    ScrollViewUIConfig scrollView;
    Vector2 itemOffset = {150.0f, 150.0f}; // spacing along (x, y)
    int wrapCount = 0;
};
