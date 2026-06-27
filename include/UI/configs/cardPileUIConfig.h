#pragma once
#include "UI/anchorPoint.h"
#include "engine/transformComponent.h"
#include "engine/spriteComponent.h"
#include "engine/hitBoxComponent.h"

struct CardPileUIConfig {
    HitBoxComponent hitBox;
    SpriteComponent sprite;
    TransformComponent transform;
    AnchorPoint drawPoint = {};
};