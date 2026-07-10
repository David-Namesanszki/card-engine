#include "core/captain.h"

Captain::Captain(std::string name, Health health, std::vector<uint32_t> deck)
    : _name(std::move(name)),
      _health(health),
      _deck(std::move(deck)) {
}

bool Captain::isDead() const {
    return _health.isDead();
}

void Captain::takeDamage(int amount) {
    _health.damage(amount);
    _healthChangedEventBus.emit({_health});
}

void Captain::announce() {
    _healthChangedEventBus.emit({_health});
}
