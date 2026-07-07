#pragma once

#include "engine/components/transformComponent.h"
#include "engine/components/spriteComponent.h"
#include "engine/components/hitBoxComponent.h"
#include "core/boardTile.h"
#include <vector>

struct BoardUIConfig {
    HitBoxComponent hitBox;
    SpriteComponent sprite;
    TransformComponent transform;

    // Same tiles the core Board is built from — the layout is defined once at
    // the composition root and fed to both sides.
    std::vector<BoardTile> tileDatas;
};
