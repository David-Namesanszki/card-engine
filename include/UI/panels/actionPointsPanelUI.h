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

    // The panel starts with no pips and builds its row from the first event;
    // a later change of max rebuilds it (energy-granting effects).
    void setActionPoints(int current, int max);

    const std::vector<Pip>& pips() const {
        return _pips;
    }

  private:
    Vector2 slotPosition(int slot) const;

    ActionPointsPanelUIConfig _config;
    int _maxActionPoints = 0;
    std::vector<Pip> _pips;
};
