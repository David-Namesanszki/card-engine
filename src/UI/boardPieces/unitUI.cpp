#include "UI/boardPieces/unitUI.h"
#include <string>

UnitUI::UnitUI(
    uint32_t id,
    HexCoord coord,
    std::string splashArt,
    std::string name,
    int health,
    int attackPower,
    int defensePower,
    int armor,
    const UnitUIConfig& config
)
    : BoardPieceUI(id, coord, std::move(splashArt), std::move(name), config.piece) {
    const BoardPieceUIConfig& base = config.piece;

    auto styleText = [&base](TextUI& out, int value, Vector2 offset) {
        out.text.text = std::to_string(value);
        out.text.fontSize = base.textFontSize;
        out.text.color = base.textColor;
        out.text.outlineColor = base.textOutlineColor;
        out.text.outlineThickness = base.textOutlineThickness;
        out.transform.position = offset;
    };
    auto stylePip = [&base](Pip& out, const std::string& texture, Vector2 offset) {
        out.sprite.texture = texture;
        out.sprite.size = base.pipSize;
        out.transform.position = offset;
    };

    styleText(healthText, health, config.healthTextOffset);
    styleText(attackPowerText, attackPower, config.attackPowerTextOffset);
    styleText(defensivePowerText, defensePower, config.defensivePowerTextOffset);
    styleText(armorText, armor, config.armorTextOffset);
    armorText.text.color = config.armorTextColor; // armor reads teal, not cream

    stylePip(healthPip, config.healthPipTexture, config.healthPipOffset);
    stylePip(attackPowerPip, config.attackPowerPipTexture, config.attackPowerPipOffset);
    stylePip(defensivePowerPip, config.defensivePowerPipTexture, config.defensivePowerPipOffset);
    stylePip(armorPip, config.armorPipTexture, config.armorPipOffset);

    transform.addChild(&healthText.transform);
    transform.addChild(&healthPip.transform);
    transform.addChild(&attackPowerText.transform);
    transform.addChild(&attackPowerPip.transform);
    transform.addChild(&defensivePowerText.transform);
    transform.addChild(&defensivePowerPip.transform);
    transform.addChild(&armorText.transform);
    transform.addChild(&armorPip.transform);
}

void UnitUI::setHealth(int health) {
    healthText.text.text = std::to_string(health);
}

void UnitUI::setAttackPower(int attackPower) {
    attackPowerText.text.text = std::to_string(attackPower);
}

void UnitUI::setDefensivePower(int defensivePower) {
    defensivePowerText.text.text = std::to_string(defensivePower);
}

void UnitUI::setArmor(int armor) {
    armorText.text.text = std::to_string(armor);
}
