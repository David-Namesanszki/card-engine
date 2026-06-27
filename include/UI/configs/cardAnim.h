#pragma once

#include "engine/animator.h"  // EasingFn
#include "engine/easing.h"

// Pure data describing how a single card travels from its current transform to a
// target anchor. Hand-edit these fields now; a future animation editor writes the
// same struct out to a file. BattleUI compiles one of these (+ from/target) into a
// runtime Animation, so adding expressive power means adding a field here and one
// line where it is compiled — never touching the event/ownership logic.
struct CardAnim {
    float    duration   = 0.30f;
    EasingFn timeEasing = Easing::easeInOut;  // remaps progress 0..1

    float    arcHeight  = 0.0f;  // perpendicular bow of the travel path, in px (sign picks a side)
    float    midScale   = 1.0f;  // extra scale multiplier at the path midpoint (1 = none)
};
