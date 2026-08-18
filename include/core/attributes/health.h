#pragma once

#include <algorithm>

struct Health {
    int current = 0;
    int max = 0;

    bool isDead() const {
        return current <= 0;
    }
    // Returns the damage left over after health is exhausted (0 if it survives).
    int damage(int amount) {
        int excess = std::max(amount - current, 0);
        current = std::max(current - amount, 0);
        return excess;
    }
    void heal(int amount) {
        current = std::min(current + amount, max);
    }
};
