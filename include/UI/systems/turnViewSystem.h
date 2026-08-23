#pragma once

#include "UI/panels/battleInfoPanelUI.h"
#include "core/types/turnType.h"
#include "core/types/battleDifficultyType.h"

#include "engine/systems/renderSystem.h"

class TurnViewSystem {
  public:
    TurnViewSystem(
        BattleInfoPanelUIConfig battleInfoPanelUI,
        BattleDifficultyType difficulty,
        TurnType whoseTurn
    );
    void advanceTurn(TurnType whoseTurn, int turnCount);

    void update(float dt);
    void render(RenderSystem& renderer);

  private:
    BattleInfoPanelUI _battleInfoPanel;
};