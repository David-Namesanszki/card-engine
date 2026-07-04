#pragma once

#include "engine/components/hitBoxComponent.h"
#include "engine/components/transformComponent.h"
#include "engine/components/spriteComponent.h"
#include "UI/configs/buttonUIConfig.h"

class ButtonUI {
  public:
    ButtonUI(ButtonUIConfig config = {});

    TransformComponent transform;
    SpriteComponent sprite;
    HitBoxComponent hitbox;
};
