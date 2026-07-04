#include "UI/unitUI.h"
#include <string>

void UnitUI::setHealth(int health) {
    healthText.text.text = std::to_string(health);
}

void UnitUI::setAttackPower(int attackPower) {
    attackPowerText.text.text = std::to_string(attackPower);
}

void UnitUI::setDefensivePower(int defensivePower) {
    defensivePowerText.text.text = std::to_string(defensivePower);
}
