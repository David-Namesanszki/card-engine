#pragma once
#include "UI/anchorPoint.h"

struct DrawPileUIConfig {
    HitBoxComponent hitBox;
    SpriteComponent sprite;
    TransformComponent transform;
    AnchorPoint drawPoint = {};
};