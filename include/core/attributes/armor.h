#pragma once

class Armor {
  public:
    explicit Armor(int defensivePower = 0)
        : _defensePower(defensivePower) {}

    void defend();
    void resetCurrentArmor();
    // Soaks damage into the current armor; returns the damage left over.
    int absorb(int damage);

    void setCurrentArmor(int armorAmount);
    void setDefensivePower(int newValue);

    int getCurrentArmor() const {
        return _currentArmor;
    }
    int getDefensivePower() const {
        return _defensePower;
    }

  private:
    int _defensePower = 0;
    int _currentArmor = 0;
};