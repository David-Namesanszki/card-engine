#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <functional>
#include "core/health.h"
#include "engine/eventBus.h"
#include "core/events/healthChangedEvent.h"

class Captain {
  public:
    Captain(std::string name, Health health, std::vector<uint32_t> deck);

    void takeDamage(int amount);
    bool isDead() const;
    // Re-emits the current state on the usual buses, so subscribers wired
    // after construction (the UI at battle start) get the opening snapshot.
    void announce();

    void onHealthChanged(std::function<void(HealthChangedEvent)> cb) {
        _healthChangedEventBus.subscribe(cb);
    }

    const std::string& getName() const {
        return _name;
    }
    const Health& getHealth() const {
        return _health;
    }
    const std::vector<uint32_t>& getDeck() const {
        return _deck;
    }

  private:
    std::string _name;
    Health _health;
    std::vector<uint32_t> _deck;

    EventBus<HealthChangedEvent> _healthChangedEventBus;
};
