#include "core/armor.h"
#include <algorithm>

void Armor::defend() {
    _currentArmor += _defensePower;
}

int Armor::absorb(int damage) {
    int soaked = std::min(damage, _currentArmor);
    _currentArmor -= soaked;
    return damage - soaked;
}

void Armor::resetCurrentArmor() {
    _currentArmor = 0;
}

void Armor::setCurrentArmor(int armorAmount) {
    _currentArmor = armorAmount;
}

void Armor::setDefensivePower(int newValue) {
    _defensePower = newValue;
}
