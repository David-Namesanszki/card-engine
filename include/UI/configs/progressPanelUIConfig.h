#pragma once

#include "engine/components/transformComponent.h"
#include "engine/components/spriteComponent.h"
#include "raylib.h"
#include <string>

struct ProgressPanelUIConfig {
    TransformComponent transform;
    SpriteComponent background;

    Vector2 pipSize = {24.0f, 24.0f};
    std::string daysPipTexture = "assets/pips/days_pip.png";
    std::string raidSectionPipTexture; // no art yet
    std::string barkOrePipTexture = "assets/pips/barkore_pip.png";
    std::string bloodSapPipTexture = "assets/pips/bloodsap_pip.png";
    std::string heartWoodPipTexture = "assets/pips/heartwood_pip.png";
};
