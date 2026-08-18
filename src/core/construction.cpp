#include "core/construction.h"

Construction::Construction(std::string name, TeamType team, Durability durability)
    : _name(std::move(name)),
      _team(team),
      _durability(durability) {
}

bool Construction::isDestroyed() const {
    return _durability.isDestroyed();
}
