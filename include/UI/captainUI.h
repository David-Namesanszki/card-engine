#pragma once

#include "engine/components/transformComponent.h"
#include "engine/components/spriteComponent.h"
#include "UI/textUI.h"
#include "UI/panels/pip.h"

class CaptainUI {
  public:
    void setHealth(int health);

    TransformComponent transform;
    SpriteComponent sprite;

    TextUI healthText;
    Pip healthPip;
};
