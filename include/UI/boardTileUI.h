#pragma once

#include <cstdint>
#include <string>
#include "engine/components/transformComponent.h"
#include "engine/components/spriteComponent.h"
#include "engine/components/hitBoxComponent.h"

enum class TeamType {
    Enemy,
    Player,
    Neutral
};

enum class BoardTileType {
    Unit,
    Construction,
    Effect,
};

class BoardTileUI {
  public:
    BoardTileUI(uint32_t id, BoardTileType type, TeamType team);

    uint32_t id() const {
        return _id;
    }

    TransformComponent transform;
    SpriteComponent sprite;
    HitBoxComponent hitbox;

  private:
    std::string getTexturePath() const;

    uint32_t _id;
    std::string _texturePath;
    BoardTileType _type;
    TeamType _team;
};