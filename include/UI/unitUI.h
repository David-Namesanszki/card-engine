#pragma once

#include <cstdint>
#include <string>
#include "engine/components/transformComponent.h"
#include "engine/components/spriteComponent.h"
#include "engine/components/hitBoxComponent.h"
#include "UI/textUI.h"
#include "UI/panels/pip.h"
#include "UI/configs/unitUIConfig.h"

class UnitUI {
  public:
    UnitUI(uint32_t id, const UnitUIConfig& config = {});

    UnitUI(const UnitUI&) = delete;
    UnitUI& operator=(const UnitUI&) = delete;

    uint32_t id() const {
        return _id;
    }

    void setHealth(int health);
    void setAttackPower(int attackPower);
    void setDefensivePower(int defensivePower);
    void setArmor(int armor);
    void setName(const std::string& name);

    TransformComponent transform;
    SpriteComponent sprite;
    HitBoxComponent hitbox;

    TextUI healthText;
    Pip healthPip;

    TextUI attackPowerText;
    Pip attackPowerPip;

    TextUI defensivePowerText;
    Pip defensivePowerPip;

    TextUI armorText;
    Pip armorPip;

    TextUI nameText;
    Pip namePip;

  private:
    uint32_t _id;
    Vector2 _nameCenter; // banner centre, for centring variable-length names
};
