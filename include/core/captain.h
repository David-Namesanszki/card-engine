#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <functional>
#include "core/attributes/health.h"
#include "core/roster.h"
#include "engine/eventBus.h"
#include "core/events/healthChangedEvent.h"
#include "core/cardPile.h"

class Captain {
  public:
    Captain(
        std::string name,
        const Health& health,
        size_t rosterSize,
        int maxActionPointCount,
        int firePointCount
    )
        : _name(std::move(name)),
          _health(health),
          _deck(),
          _roster(rosterSize),
          _currentFirePointCount(firePointCount),
          _maxActionPointCount(maxActionPointCount) {
    }

    void addCard(Card card) {
        _deck.addCard(std::move(card));
    }

    void addUnit(Unit unit) {
        _roster.addUnit(unit);
    }
    void addConstruction(Construction construction) {
        _roster.addConstruction(construction);
    }

    void takeDamage(int amount) {
        _health.damage(amount);
    }
    bool isDead() const {
        return _health.isDead();
    }

#pragma region Getters

    const std::string& getName() const {
        return _name;
    }
    const Health& getHealth() const {
        return _health;
    }
    const CardPile& getDeck() const {
        return _deck;
    }
    const Roster& getRoster() const {
        return _roster;
    }

    Roster& getRoster() {
        return _roster;
    }

    int getMaxActionPointCount() const {
        return _maxActionPointCount;
    }

    int getFirePointCount() const {
        return _currentFirePointCount;
    }

    size_t getRightHandSize() {
        return _rightHandSize;
    }
    size_t getLeftHandSize() {
        return _leftHandSize;
    }
#pragma endregion

  private:
    std::string _name;
    Health _health;
    CardPile _deck;
    Roster _roster;
    int _maxActionPointCount;
    int _currentFirePointCount;
    size_t _leftHandSize;
    size_t _rightHandSize;
};
