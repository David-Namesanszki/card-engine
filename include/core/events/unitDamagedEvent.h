#pragma once

#include <cstdint>
#include "core/attributes/health.h"

struct UnitDamagedEvent {
    uint32_t unitId;
    int currentHealth;
    int currentArmor;
};
