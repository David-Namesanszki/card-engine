#pragma once

#include <string>
#include <cstdint>
#include "core/attributes/health.h"
#include "core/attributes/armor.h"
#include "core/types/teamType.h"
#include "core/entities/boardPieces/boardPiece.h"

class Unit : public BoardPiece {
  public:
    Unit(std::string name, TeamType team, Health health, int attackPower, int defensePower);

    int takeDamage(int amount);
    void heal(int amount);
    void defend();
    bool isDead() const;

    const Health& getHealth() const {
        return _health;
    }
    const Armor& getArmor() const {
        return _armor;
    }
    int getAttackPower() const {
        return _attackPower;
    }

  private:
    Health _health;
    Armor _armor;
    int _attackPower = 0;
};
