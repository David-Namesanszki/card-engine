#pragma once

#include <cstdint>
#include "core/health.h"

struct UnitHealedEvent {
    uint32_t unitId;
    Health health;
};
