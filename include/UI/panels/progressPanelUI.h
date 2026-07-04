#pragma once

#include "engine/components/spriteComponent.h"
#include "engine/components/transformComponent.h"
#include "UI/panels/pip.h"
#include "UI/textUI.h"
#include "UI/configs/progressPanelUIConfig.h"

class ProgressPanelUI {
  public:
    ProgressPanelUI(ProgressPanelUIConfig config = {});

    void setCurrentDays(int currentDays);
    void setMaxDays(int maxDays);
    void setRaidSection(int raidSection);
    void setBarkOre(int barkOre);
    void setBloodSap(int bloodSap);
    void setHeartWood(int heartWood);

    TransformComponent transform;
    SpriteComponent background;

    Pip daysPip;
    TextUI currentDaysText;
    TextUI maxDaysText;

    Pip raidSectionPip;
    TextUI raidSectionCountText;

    Pip barkOrePip;
    TextUI barkOreCountText;

    Pip bloodSapPip;
    TextUI bloodSapCountText;

    Pip heartWoodPip;
    TextUI heartWoodCountText;
};
