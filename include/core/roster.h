#pragma once

#include <cstddef>
#include <vector>
#include "core/unit.h"
#include "core/construction.h"
#include <unordered_map>

class Roster {
  public:
    explicit Roster(size_t maxSize);

    void addUnit(std::string name, TeamType team, Health health, int attackPower, int defensePower);

    void removeUnit(uint32_t id);
    void addConstruction(std::string name, TeamType team, Durability durability);
    void removeConstruction(uint32_t id);

    const Unit* getUnit(uint32_t id) const;
    Unit* getUnit(uint32_t id);
    const Construction* getConstruction(uint32_t id) const;

  private:
    size_t _maxSize;
    std::vector<Unit> _units;
    std::vector<Construction> _constructions;
};
