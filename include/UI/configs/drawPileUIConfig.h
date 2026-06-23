#pragma once
#include "UI/anchorPoint.h"

struct DrawPileUIConfig {
    // RenderObject visual properties
    Vector2 position = {0.0f,   0.0f};
    Vector2 size     = {400.0f, 150.0f};
    Color   color    = {30, 30, 50, 200};
    int     layer    = 0;

    AnchorPoint drawPoint = {{0, 0}, 0.0f, 1.0f};
};