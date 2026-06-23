#pragma once

#include <vector>
#include <cstdint>

class CardPile {
public:
    CardPile(int limit = -1);
    bool addCard(uint32_t card);
    bool removeCard(uint32_t card);
    bool isFull() const;
    bool isEmpty() const;
    void shuffle();
    bool transfer(CardPile& other);
    uint32_t popLast();
    void empty();
    std::vector<uint32_t> clearOut();
    int size() const { return static_cast<int>(_cards.size()); }
    const std::vector<uint32_t>& getCards() const { return _cards; }

protected:
    int _limit;
    std::vector<uint32_t> _cards;
};