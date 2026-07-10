#pragma once

#include <cstdint>
#include "core/health.h"

struct UnitDamagedEvent {
    uint32_t unitId;
    Health health;
    int currentArmor;
};
