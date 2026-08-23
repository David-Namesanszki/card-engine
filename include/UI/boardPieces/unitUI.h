#pragma once

#include <cstdint>
#include <string>
#include "UI/boardPieces/boardPieceUI.h"
#include "UI/configs/unitUIConfig.h"
#include "UI/panels/pip.h"
#include "UI/textUI.h"

class UnitUI : public BoardPieceUI {
  public:
    UnitUI(
        uint32_t id,
        HexCoord coord,
        std::string splashArt,
        std::string name,
        int health,
        int attackPower,
        int defensePower,
        int armor,
        const UnitUIConfig& config = {}
    );

    void setHealth(int health);
    void setAttackPower(int attackPower);
    void setDefensivePower(int defensivePower);
    void setArmor(int armor);

    TextUI healthText;
    Pip healthPip;

    TextUI attackPowerText;
    Pip attackPowerPip;

    TextUI defensivePowerText;
    Pip defensivePowerPip;

    TextUI armorText;
    Pip armorPip;
};
