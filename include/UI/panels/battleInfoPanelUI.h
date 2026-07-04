#pragma once

#include "engine/components/spriteComponent.h"
#include "engine/components/transformComponent.h"
#include "UI/panels/pip.h"
#include "UI/textUI.h"
#include "UI/configs/battleInfoPanelUIConfig.h"
#include <string>

class BattleInfoPanelUI {
  public:
    BattleInfoPanelUI(BattleInfoPanelUIConfig config = {});

    void setDifficulty(const std::string& difficulty);
    void setWhoseTurn(const std::string& whoseTurn);
    void setBattleLength(int battleLength);

    TransformComponent transform;
    SpriteComponent background;

    Pip difficultyPip;
    TextUI difficultyText;

    Pip whoseTurnPip;
    TextUI whoseTurnText;

    Pip battleLengthPip;
    TextUI battleLengthText;
};
