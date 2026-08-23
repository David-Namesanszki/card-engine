#pragma once

#include "engine/eventBus.h"
#include "core/events/actionPointsRefilledEvent.h"
#include "core/events/resourcesSpentEvent.h"

struct ActionPoints {
    int current = 0;
    int max = 0;

    bool canAfford(int cost) const {
        return cost <= current;
    }
    void spend(int cost) {
        current -= cost;
    }
    void refill() {
        current = max;
    }
};

struct FirePoints {
    int original = 0;
    int current = 0;

    bool canAfford(int cost) const {
        return cost <= current;
    }
    void spend(int cost) {
        current -= cost;
    }
    void refill() {
        current = original;
    }
};

class BattleResourceSystem {
  public:
    BattleResourceSystem(int maxActionPoints, int firePoints)
        : _actionPoints{maxActionPoints, maxActionPoints},
          _firePoints{firePoints, firePoints} {
    }

    void refillActionPoints();

    bool canAfford(int actionCost, int fireCost);
    void spendResources(int actionCost, int fireCost);

    void onActionPointsRefilled(std::function<void(ActionPointsRefilledEvent)> cb) {
        _actionPointsRefilled.subscribe(cb);
    }
    void onResourcesSpent(std::function<void(ResourcesSpentEvent)> cb) {
        _resourcesSpent.subscribe(cb);
    }

  private:
    ActionPoints _actionPoints;
    FirePoints _firePoints;

    EventBus<ActionPointsRefilledEvent> _actionPointsRefilled;
    EventBus<ResourcesSpentEvent> _resourcesSpent;
};