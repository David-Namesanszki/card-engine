#pragma once

// One payment, both resources — spending is a single operation, so it is a
// single event and no subscriber can observe half of it applied.
// The costs are carried alongside the remainders so a subscriber can show what
// was paid ("-2") without diffing against remembered state.
struct ResourcesSpentEvent {
    int actionCost;
    int fireCost;
    int actionPointsRemaining;
    int firePointsRemaining;
};
