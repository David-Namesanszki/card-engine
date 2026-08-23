#pragma once

// Action points were restored to their maximum. Carries the resulting state so
// subscribers never have to track the value themselves.
struct ActionPointsRefilledEvent {
    int current;
    int max;
};
