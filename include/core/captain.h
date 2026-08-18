#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <functional>
#include "core/attributes/health.h"
#include "core/roster.h"
#include "engine/eventBus.h"
#include "core/events/healthChangedEvent.h"
#include "core/deck.h"

class Captain {
  public:
    Captain(
        std::string name,
        const Health& health,
        size_t rosterSize,
        int maxActionPointCount,
        int firePointCount
    );

    void addCard(
        CardType type,
        const std::string& name,
        const std::string& description,
        const std::string& splashArt,
        int fireCost
    );

    void addUnit(
        const std::string& name,
        TeamType team,
        Health health,
        int attackPower,
        int defensePower
    );
    void addConstruction(const std::string& name, TeamType team, Durability durability);

    void takeDamage(int amount);
    bool isDead() const;

    void onHealthChanged(std::function<void(HealthChangedEvent)> cb) {
        _healthChangedEventBus.subscribe(cb);
    }

#pragma region Getters

    const std::string& getName() const {
        return _name;
    }
    const Health& getHealth() const {
        return _health;
    }
    const Deck& getDeck() const {
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

#pragma endregion

  private:
    std::string _name;
    Health _health;
    Deck _deck;
    Roster _roster;
    int _maxActionPointCount;
    int _currentFirePointCount;

    EventBus<HealthChangedEvent> _healthChangedEventBus;
};
