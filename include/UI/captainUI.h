#pragma once

#include "engine/components/transformComponent.h"
#include "engine/components/spriteComponent.h"
#include "UI/textUI.h"
#include "UI/panels/pip.h"
#include "UI/configs/captainUIConfig.h"

class CaptainUI {
  public:
    CaptainUI(CaptainUIConfig config = {});

    void setHealth(int current, int max);

    TransformComponent transform;
    SpriteComponent sprite;

    TextUI healthText;
    Pip healthPip;
};
