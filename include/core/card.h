#pragma once

#include <string>
#include <vector>
#include <functional>

enum class CardType {
    Command,
    Weather,
    Nightmare,
    Dream,
    Fire
};

static uint32_t nextId;

class Card {
  public:
    Card(
        CardType type,
        std::string name,
        std::string description,
        std::string splashArt,
        int fireCost
    )
        : _id(nextId++),
          _type(type),
          _name(std::move(name)),
          _description(std::move(description)),
          _splashArt(std::move(splashArt)),
          _fireCost(fireCost) {
    }

    uint32_t getId() const {
        return _id;
    }

  private:
    uint32_t _id;
    CardType _type;
    std::string _name;
    std::string _description;
    std::string _splashArt;
    int _fireCost;
};