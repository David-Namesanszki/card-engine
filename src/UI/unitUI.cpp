#include "UI/unitUI.h"
#include <string>

UnitUI::UnitUI(uint32_t id, const UnitUIConfig& config)
    : _id(id),
      _nameCenter(config.namePipOffset) {
    sprite.size = config.spriteSize;
    hitbox.setRectangle(config.spriteSize);

    for (TextUI* text : {&healthText, &attackPowerText, &defensivePowerText, &armorText, &nameText}) {
        // Deliberately implausible placeholder: displayed values belong to the
        // core and arrive via events — a visible "?" means an emit went missing.
        text->text.text = "?";
        text->text.fontSize = config.textFontSize;
        text->text.color = config.textColor;
        text->text.outlineColor = config.textOutlineColor;
        text->text.outlineThickness = config.textOutlineThickness;
    }
    armorText.text.color = config.armorTextColor;
    nameText.text.fontSize = config.nameFontSize;

    namePip.sprite.texture = config.namePipTexture;
    namePip.sprite.size = config.namePipSize;
    namePip.transform.position = config.namePipOffset;

    healthPip.sprite.texture = config.healthPipTexture;
    healthPip.sprite.size = config.pipSize;
    attackPowerPip.sprite.texture = config.attackPowerPipTexture;
    attackPowerPip.sprite.size = config.pipSize;
    defensivePowerPip.sprite.texture = config.defensivePowerPipTexture;
    defensivePowerPip.sprite.size = config.pipSize;
    armorPip.sprite.texture = config.armorPipTexture;
    armorPip.sprite.size = config.pipSize;

    attackPowerPip.transform.position = config.attackPowerPipOffset;
    attackPowerText.transform.position = config.attackPowerTextOffset;
    defensivePowerPip.transform.position = config.defensivePowerPipOffset;
    defensivePowerText.transform.position = config.defensivePowerTextOffset;
    healthPip.transform.position = config.healthPipOffset;
    healthText.transform.position = config.healthTextOffset;
    armorPip.transform.position = config.armorPipOffset;
    armorText.transform.position = config.armorTextOffset;

    transform.addChild(&healthText.transform);
    transform.addChild(&healthPip.transform);
    transform.addChild(&attackPowerText.transform);
    transform.addChild(&attackPowerPip.transform);
    transform.addChild(&defensivePowerText.transform);
    transform.addChild(&defensivePowerPip.transform);
    transform.addChild(&armorText.transform);
    transform.addChild(&armorPip.transform);
    transform.addChild(&nameText.transform);
    transform.addChild(&namePip.transform);
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

void UnitUI::setName(const std::string& name) {
    nameText.text.text = name;
    // DrawText anchors top-left, so centre the variable-length name on the
    // banner by measuring it.
    nameText.transform.position = {
        _nameCenter.x - MeasureText(name.c_str(), nameText.text.fontSize) / 2.0f,
        _nameCenter.y - nameText.text.fontSize / 2.0f
    };
}
