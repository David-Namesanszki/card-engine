#pragma once

#include <string>
#include <cstdint>
#include "core/attributes/health.h"
#include "core/attributes/armor.h"
#include "core/types/teamType.h"

class Unit {
  public:
    Unit(std::string name, TeamType team, Health health, int attackPower, int defensePower);

    // Armor soaks first, then health; returns the overkill damage left after
    // both are exhausted (0 if the unit survives).
    int takeDamage(int amount);
    void heal(int amount);
    void defend();
    bool isDead() const;

    const std::string& getName() const {
        return _name;
    }
    TeamType getTeam() const {
        return _team;
    }
    const Health& getHealth() const {
        return _health;
    }
    const Armor& getArmor() const {
        return _armor;
    }
    int getAttackPower() const {
        return _attackPower;
    }
    uint32_t getId() const {
        return _id;
    }
    void setId(uint32_t id) {
        _id = id;
    }

  private:
    std::string _name;
    TeamType _team = TeamType::Player;
    Health _health;
    Armor _armor;
    int _attackPower = 0;
    uint32_t _id = 0;
};
