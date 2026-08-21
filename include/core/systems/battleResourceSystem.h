#pragma once

#include "engine/eventBus.h"
#include "core/events/currentActionPointsChangedEvent.h"
#include "core/events/currentFirePointsChangedEvent.h"
#include "core/events/maxActionPointsChangedEvent.h"

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
    void changeCurrentActionPoints(int newValue);

    void changeCurrentFirePoints(int newValue);

    bool canAfford(int actionCost, int fireCost);
    void spend(int actionCost, int fireCost);

    void onCurrentFirePointsChanged(std::function<void(CurrentFirePointsChangedEvent)> cb) {
        _currentFirePointsChanged.subscribe(cb);
    }
    void onCurrentActionPointsChanged(std::function<void(CurrentActionPointsChangedEvent)> cb) {
        _currentActionPointsChanged.subscribe(cb);
    }

  private:
    ActionPoints _actionPoints;
    FirePoints _firePoints;

    EventBus<CurrentFirePointsChangedEvent> _currentFirePointsChanged;
    EventBus<CurrentActionPointsChangedEvent> _currentActionPointsChanged;
};