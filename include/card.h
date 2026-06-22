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

class Card {
public:
    Card(uint32_t id,
        CardType type,
        std::string name, 
        std::string description, 
        std::string splashArt, 
        int fireCost);
private:
    uint32_t _id;
    CardType _type;
    std::string _name;
    std::string _description;
    std::string _splashArt;
    int _fireCost;
};