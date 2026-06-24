#pragma once

#include <cstdint>
#include "engine/transformComponent.h"
#include "engine/spriteComponent.h"
#include "engine/hitBoxComponent.h"

class CardUI {
  public:
    explicit CardUI(uint32_t id) : _id(id) {
    }
    uint32_t id() const {
        return _id;
    }

    TransformComponent transform;
    SpriteComponent sprite;
    HitBoxComponent hitbox;

  private:
    uint32_t _id;
};
