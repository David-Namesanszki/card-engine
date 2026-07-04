#include "UI/panels/battleInfoPanelUI.h"

BattleInfoPanelUI::BattleInfoPanelUI(BattleInfoPanelUIConfig config)
    : transform(config.transform),
      background(config.background) {
}

void BattleInfoPanelUI::setDifficulty(const std::string& difficulty) {
    difficultyText.text.text = difficulty;
}

void BattleInfoPanelUI::setWhoseTurn(const std::string& whoseTurn) {
    whoseTurnText.text.text = whoseTurn;
}

void BattleInfoPanelUI::setBattleLength(int battleLength) {
    battleLengthText.text.text = std::to_string(battleLength);
}
