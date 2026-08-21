#pragma once

#include <string>
#include <vector>
#include <functional>
#include "entity.h"

#include "core/types/cardType.h"

class Card : public Entity {
  public:
    Card(
        CardType type,
        std::string name,
        std::string description,
        std::string splashArt,
        int fireCost
    )
        : _type(type),
          _name(std::move(name)),
          _description(std::move(description)),
          _splashArt(std::move(splashArt)),
          _fireCost(fireCost) {
    }

  private:
    CardType _type;
    std::string _name;
    std::string _description;
    std::string _splashArt;
    int _fireCost;
};