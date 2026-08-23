#pragma once

#include <cstdint>
#include "core/attributes/health.h"

struct UnitDamagedEvent {
    uint32_t id;
    int currentHealth;
    int currentArmor;
};
