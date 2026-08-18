#include "core/roster.h"
#include <algorithm>

Roster::Roster(size_t maxSize)
    : _maxSize(maxSize) {
}

void Roster::addUnit(
    std::string name,
    TeamType team,
    Health health,
    int attackPower,
    int defensePower
) {
    if (_units.size() + _constructions.size() >= _maxSize)
        return;
    _units.emplace_back(std::move(name), team, health, attackPower, defensePower);
}

void Roster::removeUnit(uint32_t id) {
    auto it = std::find_if(_units.begin(), _units.end(), [id](const Unit& unit) {
        return unit.getId() == id;
    });

    if (it != _units.end())
        _units.erase(it);
}

void Roster::addConstruction(std::string name, TeamType team, Durability durability) {
    if (_units.size() + _constructions.size() >= _maxSize)
        return;
    _constructions.emplace_back(std::move(name), team, durability);
}

void Roster::removeConstruction(uint32_t id) {
    auto it = std::find_if(
        _constructions.begin(), _constructions.end(), [id](const Construction& construction) {
            return construction.getId() == id;
        }
    );

    if (it != _constructions.end())
        _constructions.erase(it);
}

const Unit* Roster::getUnit(uint32_t id) const {
    auto it = std::find_if(_units.begin(), _units.end(), [id](const Unit& unit) {
        return unit.getId() == id;
    });
    return it == _units.end() ? nullptr : &*it;
}

Unit* Roster::getUnit(uint32_t id) {
    auto it = std::find_if(_units.begin(), _units.end(), [id](const Unit& unit) {
        return unit.getId() == id;
    });
    return it == _units.end() ? nullptr : &*it;
}

const Construction* Roster::getConstruction(uint32_t id) const {
    auto it = std::find_if(
        _constructions.begin(), _constructions.end(), [id](const Construction& construction) {
            return construction.getId() == id;
        }
    );
    return it == _constructions.end() ? nullptr : &*it;
}