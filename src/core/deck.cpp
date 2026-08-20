#include "core/deck.h"

void Deck::addCard(
    CardType type,
    const std::string& name,
    const std::string& description,
    const std::string& splashArt,
    int fireCost
) {
    _cards.emplace_back(type, name, description, splashArt, fireCost);
};

bool Deck::removeCard(uint32_t cardId) {
    auto it = std::find_if(_cards.begin(), _cards.end(), [cardId](const Card& card) {
        return card.getId() == cardId;
    });

    if (it == _cards.end())
        return false;
    _cards.erase(it);
    return true;
}

const Card* Deck::getCard(uint32_t id) const {
    auto it = find_if(_cards.begin(), _cards.end(), [id](const Card& card) {
        return card.getId() == id;
    });

    return it == _cards.end() ? nullptr : &*it;
}