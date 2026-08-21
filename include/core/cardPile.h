#pragma once

#include <vector>
#include <cstdint>
#include "core/entities/cards/card.h"

class CardPile {
  public:
    CardPile(int limit = -1);
    bool addCard(Card card);
    bool removeCard(uint32_t card);
    bool isFull() const;
    bool isEmpty() const;
    void shuffle();
    bool transfer(CardPile& other);
    Card popLast();
    void empty();
    std::vector<Card> clearOut();
    size_t size() const {
        return _cards.size();
    }
    const std::vector<Card>& getCards() const {
        return _cards;
    }

  protected:
    int _limit;
    std::vector<Card> _cards;
};