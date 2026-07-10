#include "UI/panels/battleInfoPanelUI.h"

BattleInfoPanelUI::BattleInfoPanelUI(BattleInfoPanelUIConfig config)
    : transform(config.transform),
      background(config.background) {
    // Three rows stacked down the 128x216 panel; the real values arrive via
    // events, so the placeholders are deliberately implausible.
    transform.addChild(&difficultyText.transform);
    transform.addChild(&whoseTurnText.transform);
    transform.addChild(&battleLengthText.transform);

    difficultyText.transform.position = {-30.0f, -66.0f};
    difficultyText.text.text = "?";

    whoseTurnText.transform.position = {-30.0f, -6.0f};
    whoseTurnText.text.text = "?";

    battleLengthText.transform.position = {-30.0f, 54.0f};
    battleLengthText.text.text = "?";
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
