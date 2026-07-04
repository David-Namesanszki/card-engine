#pragma once

#include "engine/components/spriteComponent.h"
#include "engine/components/transformComponent.h"
#include "UI/configs/actionPointsPanelUIConfig.h"
#include "UI/panels/pip.h"
#include <vector>

class ActionPointsPanelUI {
  public:
    ActionPointsPanelUI(ActionPointsPanelUIConfig config = {});

    TransformComponent transform;
    SpriteComponent sprite;

    void setSpent(int spentCount);

    const std::vector<Pip>& pips() const {
        return _pips;
    }

  private:
    Vector2 slotPosition(int slot) const;

    ActionPointsPanelUIConfig _config;
    std::vector<Pip> _pips;
};
