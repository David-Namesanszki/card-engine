#pragma once

#include <cstddef>
#include "core/types/turnType.h"

// Play passed to the other side. Carries the resulting state of both fields
// because they change as one operation; `turnCount` only moves when a full
// round completes, so subscribers may see it unchanged.
struct TurnAdvancedEvent {
    TurnType whoseTurn;
    size_t turnCount;
};
