#pragma once

#include "core/health.h"
#include "core/types/teamType.h"

struct Unit {
    TeamType team = TeamType::Player;
    Health health;
    int defensivePower = 0;
    int currentArmor = 0;
    int attackPower = 0;
};
