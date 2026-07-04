#pragma once

#include <cstdint>
#include "engine/components/transformComponent.h"
#include "engine/components/spriteComponent.h"
#include "engine/components/hitBoxComponent.h"

class CardUI {
  public:
    explicit CardUI(uint32_t id)
        : _id(id) {
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
