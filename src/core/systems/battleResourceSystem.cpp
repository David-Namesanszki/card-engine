#include "core/systems/battleResourceSystem.h"

void BattleResourceSystem::refillActionPoints() {
    _actionPoints.refill();
    _actionPointsRefilled.emit(ActionPointsRefilledEvent{_actionPoints.current, _actionPoints.max});
}

bool BattleResourceSystem::canAfford(int actionCost, int fireCost) {
    return _actionPoints.canAfford(actionCost) && _firePoints.canAfford(fireCost);
}

// Both resources move as one payment, so they leave as one event — a subscriber
// can never see the action points debited but the fire points not yet.
void BattleResourceSystem::spendResources(int actionCost, int fireCost) {
    if (!canAfford(actionCost, fireCost))
        return;

    _actionPoints.spend(actionCost);
    _firePoints.spend(fireCost);

    _resourcesSpent.emit(ResourcesSpentEvent{
        actionCost,
        fireCost,
        _actionPoints.current,
        _firePoints.current
    });
}
