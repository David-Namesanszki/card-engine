#include "core/unit.h"

Unit::Unit(std::string name, TeamType team, Health health, int attackPower, int defensePower)
    : _name(std::move(name)),
      _team(team),
      _health(health),
      _armor(defensePower),
      _attackPower(attackPower) {
}

int Unit::takeDamage(int amount) {
    return _health.damage(_armor.absorb(amount));
}

void Unit::heal(int amount) {
    _health.heal(amount);
}

void Unit::defend() {
    _armor.defend();
}

bool Unit::isDead() const {
    return _health.isDead();
}
