#pragma once

#include "engine/components/spriteComponent.h"
#include "engine/components/transformComponent.h"
#include "UI/textUI.h"
#include "UI/panels/pip.h"
#include "UI/configs/fireResourcePanelUIConfig.h"

class FireResourcePanelUI {
  public:
    FireResourcePanelUI(FireResourcePanelUIConfig config = {});

    TransformComponent transform;
    SpriteComponent background;

    void setFireCount(int fireCount);

    Pip firePip;
    TextUI fireCountText;
};
