#include "UI/systems/battleResourceViewSystem.h"

BattleResourceViewSystem::BattleResourceViewSystem(
    FireResourcePanelUIConfig firePointsPanel,
    ActionPointsPanelUIConfig actionPointsPanel,
    int maxActionPoints,
    int firePoints
)
    : _actionPointsPanel(actionPointsPanel),
      _firePointsPanel(firePointsPanel) {
    _actionPointsPanel.setCurrentActionPoints(maxActionPoints);
    _actionPointsPanel.setMaxActionPoints(maxActionPoints);
    _firePointsPanel.setFireCount(firePoints);
}

void BattleResourceViewSystem::update(float dt) {
    // animations
}

void BattleResourceViewSystem::render(RenderSystem& renderer) {
    renderer.renderResourcePanel(_firePointsPanel);
    renderer.renderActionPointsPanel(_actionPointsPanel);
}

void BattleResourceViewSystem::refillActionPoints(int actionPoints) {
    _actionPointsPanel.setCurrentActionPoints(actionPoints);
}
void BattleResourceViewSystem::spendResources(
    int actionPointsSpent,
    int actionPointsRemaning,
    int firePointsSpent,
    int firePointsRemaning
) {
    _actionPointsPanel.setCurrentActionPoints(actionPointsRemaning);
    _firePointsPanel.setFireCount(firePointsRemaning);
}