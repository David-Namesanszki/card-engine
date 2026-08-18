#include "core/captain.h"

Captain::Captain(
    std::string name,
    const Health& health,
    size_t rosterSize,
    int firePointCount,
    int actionPointCount
)
    : _name(std::move(name)),
      _health(health),
      _deck(),
      _roster(rosterSize),
      _currentFirePointCount(firePointCount),
      _maxActionPointCount(actionPointCount) {
}

void Captain::addCard(
    CardType type,
    const std::string& name,
    const std::string& description,
    const std::string& splashArt,
    int fireCost
) {
    _deck.addCard(type, name, description, splashArt, fireCost);
}

void Captain::addUnit(
    const std::string& name,
    TeamType team,
    Health health,
    int attackPower,
    int defensePower
) {
    _roster.addUnit(name, team, health, attackPower, defensePower);
}

void Captain::addConstruction(const std::string& name, TeamType team, Durability durability) {
    _roster.addConstruction(name, team, durability);
}

bool Captain::isDead() const {
    return _health.isDead();
}

void Captain::takeDamage(int amount) {
    _health.damage(amount);
    _healthChangedEventBus.emit({_health});
}
