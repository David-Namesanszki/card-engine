#pragma once

#include <string>
#include <cstdint>
#include "core/attributes/durability.h"
#include "core/types/teamType.h"

class Construction {
  public:
    Construction(std::string name, TeamType team, Durability durability);

    bool isDestroyed() const;

    const std::string& getName() const {
        return _name;
    }
    TeamType getTeam() const {
        return _team;
    }
    const Durability& getDurability() const {
        return _durability;
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
    Durability _durability;
    uint32_t _id = 0;
};
