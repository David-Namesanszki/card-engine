#pragma once

#include "engine/components/transformComponent.h"
#include "engine/components/spriteComponent.h"
#include "raylib.h"
#include <string>

struct CaptainUIConfig {
    TransformComponent transform;
    SpriteComponent sprite;

    Vector2 pipSize = {24.0f, 24.0f};
    std::string healthPipTexture = "assets/pips/health_pip.png";
};
