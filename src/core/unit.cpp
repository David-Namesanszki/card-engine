#include "core/entities/boardPieces/unit.h"

Unit::Unit(std::string name, TeamType team, Health health, int attackPower, int defensePower)
    : BoardPiece(name, team, BoardPieceType::Unit),
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
