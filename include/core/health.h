#pragma once

#include <algorithm>

struct Health {
    int current = 0;
    int max = 0;

    bool isDead() const {
        return current <= 0;
    }
    void damage(int amount) {
        current = std::max(current - amount, 0);
    }
    void heal(int amount) {
        current = std::min(current + amount, max);
    }
};
