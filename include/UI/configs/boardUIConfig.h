#pragma once

#include "engine/components/transformComponent.h"
#include "engine/components/spriteComponent.h"
#include "engine/components/hitBoxComponent.h"
#include "UI/boardTileUI.h"
#include <vector>

struct BoardTileData {
    HexCoord coord;
    BoardTileType type;
    TeamType team;
};

struct BoardUIConfig {
    HitBoxComponent hitBox;
    SpriteComponent sprite;
    TransformComponent transform;

    std::vector<BoardTileData> tileDatas = {
        {{-1, 1}, BoardTileType::Unit, TeamType::Player},
        {{-1, 0}, BoardTileType::Unit, TeamType::Enemy},
        {{0, 0}, BoardTileType::Effect, TeamType::Neutral},
        {{1, 0}, BoardTileType::Unit, TeamType::Player},
        {{1, -1}, BoardTileType::Unit, TeamType::Enemy},
    };
};