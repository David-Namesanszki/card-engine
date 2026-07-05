#pragma once

#include <cstdint>
#include "engine/components/transformComponent.h"
#include "engine/components/spriteComponent.h"
#include "engine/components/hitBoxComponent.h"
#include "UI/textUI.h"
#include "UI/panels/pip.h"

class UnitUI {
  public:
    // Stat texts are left unparented (unlike the panels) because UnitUI gets
    // copied in and out of containers, and a transform parented to this
    // object's own transform would dangle in the copy. Whoever places a unit
    // on the board must position the texts/pips in world space alongside it.
    explicit UnitUI(uint32_t id)
        : _id(id) {
        healthText.text.text = "10";
        attackPowerText.text.text = "2";
        defensivePowerText.text.text = "1";

        healthPip.sprite.texture = "assets/pips/health_pip.png";
        healthPip.sprite.size = {24.0f, 24.0f};
        attackPowerPip.sprite.size = {24.0f, 24.0f}; // no art yet
        defensivePowerPip.sprite.size = {24.0f, 24.0f}; // no art yet
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
