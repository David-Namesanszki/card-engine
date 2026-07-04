#pragma once
#include "UI/anchorPoint.h"
#include "engine/components/transformComponent.h"
#include "engine/components/spriteComponent.h"
#include "engine/components/hitBoxComponent.h"

struct CardPileUIConfig {
    HitBoxComponent hitBox;
    SpriteComponent sprite;
    TransformComponent transform;
    AnchorPoint drawPoint = {};
};