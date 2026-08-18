#pragma once

#include <cstdint>
#include "core/attributes/health.h"

struct UnitHealedEvent {
    uint32_t unitId;
    Health health;
};
