#include "cardPile.h"
#include <algorithm>
#include <random>

CardPile::CardPile(int limit) : _limit(limit) {}

bool CardPile::addCard(uint32_t card) {
    if (_limit != -1 && _cards.size() >= static_cast<size_t>(_limit))
        return false;

    _cards.push_back(card);
    return true;
}

bool CardPile::removeCard(uint32_t card) {
    auto it = std::find(_cards.begin(), _cards.end(), card);

    if (it == _cards.end())
        return false;

    _cards.erase(it);
    return true;
}

bool CardPile::isFull() const {
    return _limit != -1 && _cards.size() >= static_cast<size_t>(_limit);
}

bool CardPile::isEmpty() const {
    return _cards.empty();
}

void CardPile::shuffle() {
    static std::mt19937 rng(std::random_device{}());
    std::shuffle(_cards.begin(), _cards.end(), rng);
}

bool CardPile::transfer(CardPile& other) {
    for (auto card : _cards) {
        other.addCard(card);
    }
    empty();
    return true;
}

uint32_t CardPile::popLast() {
    uint32_t card = _cards.back();
    _cards.pop_back();
    return card;
}

std::vector<uint32_t> CardPile::clearOut() {
    std::vector<uint32_t> cards = _cards;
    empty();
    return cards;
}

void CardPile::empty() {
    _cards.clear();
}
