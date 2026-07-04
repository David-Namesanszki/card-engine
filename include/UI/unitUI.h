#pragma once

#include <cstdint>
#include "engine/components/transformComponent.h"
#include "engine/components/spriteComponent.h"
#include "engine/components/hitBoxComponent.h"
#include "UI/textUI.h"
#include "UI/panels/pip.h"

class UnitUI {
  public:
    explicit UnitUI(uint32_t id)
        : _id(id) {
    }
    uint32_t id() const {
        return _id;
    }

    void setHealth(int health);
    void setAttackPower(int attackPower);
    void setDefensivePower(int defensivePower);

    TransformComponent transform;
    SpriteComponent sprite;
    HitBoxComponent hitbox;

    TextUI healthText;
    Pip healthPip;

    TextUI attackPowerText;
    Pip attackPowerPip;

    TextUI defensivePowerText;
    Pip defensivePowerPip;

  private:
    uint32_t _id;
};
