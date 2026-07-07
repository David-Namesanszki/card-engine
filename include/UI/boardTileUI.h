#pragma once

#include <cstdint>
#include "core/boardTile.h"
#include "engine/components/transformComponent.h"
#include "engine/components/spriteComponent.h"
#include "engine/components/hitBoxComponent.h"

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
    uint32_t _id;
};