#pragma once

#include "raylib.h"
#include <string>

// Static presentation shared by every unit. Readout offsets are local to the
// unit's transform, values drawn on their pips: attack power on the left
// edge, defensive power on the right, health bottom centre with armor just
// above-right of it. The sprite texture varies per unit, so it stays a
// placeUnit argument.
struct UnitUIConfig {
    Vector2 spriteSize = {187.0f, 135.0f};
    Vector2 pipSize = {25.0f, 25.0f};

    int textFontSize = 16;
    Color textColor = {245, 235, 210, 255};      // cream, like the mockup numbers
    Color armorTextColor = {110, 230, 210, 255}; // teal armor value
    Color textOutlineColor = {25, 25, 30, 255};
    int textOutlineThickness = 1;

    std::string healthPipTexture = "assets/pips/health_pip.png";
    std::string attackPowerPipTexture = "assets/pips/attack_power_pip.png";
    std::string defensivePowerPipTexture = "assets/pips/defense_power_pip.png";
    std::string armorPipTexture = "assets/pips/armor_pip.png";

    // Name banner above the sprite; the text centres itself on the banner.
    std::string namePipTexture = "assets/pips/name_pip.png";
    Vector2 namePipSize = {56.0f, 16.0f};
    Vector2 namePipOffset = {0.0f, -40.0f};
    int nameFontSize = 10;

    Vector2 attackPowerPipOffset = {-36.0f, -6.0f};
    Vector2 attackPowerTextOffset = {-41.0f, -14.0f};
    Vector2 defensivePowerPipOffset = {36.0f, -6.0f};
    Vector2 defensivePowerTextOffset = {31.0f, -14.0f};
    Vector2 healthPipOffset = {0.0f, 34.0f};
    Vector2 healthTextOffset = {-5.0f, 26.0f};
    Vector2 armorPipOffset = {20.0f, 24.0f};
    Vector2 armorTextOffset = {15.0f, 16.0f};
};
