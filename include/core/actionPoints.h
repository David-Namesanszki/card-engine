#pragma once

struct ActionPoints {
    int current = 0;
    int max = 0;

    bool canAfford(int cost) const {
        return cost <= current;
    }
    void spend(int cost) {
        current -= cost;
    }
    void refill() {
        current = max;
    }
};
