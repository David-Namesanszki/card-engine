#pragma once

#include "core/card.h"
#include <vector>

class Deck {
  public:
    Deck() {};
    void addCard(
        CardType type,
        const std::string& name,
        const std::string& description,
        const std::string& splashArt,
        int fireCost
    );
    bool removeCard(uint32_t cardId);
    int size() const {
        return static_cast<int>(_cards.size());
    }
    const std::vector<Card>& getCards() const {
        return _cards;
    }

    const Card* getCard(uint32_t id) const;

  private:
    std::vector<Card> _cards;
};