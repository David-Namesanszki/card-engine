#include "core/systems/battleResourceSystem.h"

void BattleResourceSystem::refillActionPoints() {
    _actionPoints.refill();
    _currentActionPointsChanged.emit(CurrentActionPointsChangedEvent{_actionPoints.current});
}

void BattleResourceSystem::changeCurrentActionPoints(int newValue) {
    _actionPoints.current = newValue;
    _currentActionPointsChanged.emit(CurrentActionPointsChangedEvent{_actionPoints.current});
}

void BattleResourceSystem::changeCurrentFirePoints(int newValue) {
    _firePoints.current = newValue;
    _currentFirePointsChanged.emit(CurrentFirePointsChangedEvent{_firePoints.current});
}

bool BattleResourceSystem::canAfford(int actionCost, int fireCost) {
    return _actionPoints.canAfford(actionCost) && _firePoints.canAfford(fireCost);
}

void BattleResourceSystem::spend(int actionCost, int fireCost) {
    if (canAfford(actionCost, fireCost)) {
        _actionPoints.spend(actionCost);
        _firePoints.spend(fireCost);
    }
}