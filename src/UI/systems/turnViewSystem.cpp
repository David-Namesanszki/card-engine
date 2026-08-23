#include "UI/systems/turnViewSystem.h"
#include "UI/labels.h"

TurnViewSystem::TurnViewSystem(
    BattleInfoPanelUIConfig battleInfoPanelUI,
    BattleDifficultyType difficulty,
    TurnType whoseTurn
)
    : _battleInfoPanel(battleInfoPanelUI) {
    _battleInfoPanel.setDifficulty(difficultyLabel(difficulty));
    _battleInfoPanel.setWhoseTurn(turnTypeLabel(whoseTurn));
    _battleInfoPanel.setBattleLength(0);
}

void TurnViewSystem::update(float dt) {
    // animations
}

void TurnViewSystem::render(RenderSystem& renderer) {
    renderer.renderBattleInfoPanel(_battleInfoPanel);
}

void TurnViewSystem::advanceTurn(TurnType whoseTurn, int turnCount) {
    _battleInfoPanel.setWhoseTurn(turnTypeLabel(whoseTurn));
    _battleInfoPanel.setBattleLength(turnCount);
}