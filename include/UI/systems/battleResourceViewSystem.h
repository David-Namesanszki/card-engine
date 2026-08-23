#pragma once

#include "UI/panels/fireResourcePanelUI.h"
#include "UI/panels/actionPointsPanelUI.h"

#include "engine/systems/renderSystem.h"

class BattleResourceViewSystem {
  public:
    BattleResourceViewSystem(
        FireResourcePanelUIConfig firePointsPanel,
        ActionPointsPanelUIConfig actionPointsPanel,
        int maxActionPoints,
        int firePoints
    );
    void refillActionPoints(int actionPoints);
    void spendResources(
        int actionPointsSpent,
        int actionPointsRemaning,
        int firePointsSpent,
        int firePointsRemaning
    );

    void update(float dt);
    void render(RenderSystem& renderer);

  private:
    FireResourcePanelUI _firePointsPanel;
    ActionPointsPanelUI _actionPointsPanel;
};