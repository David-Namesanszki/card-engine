#pragma once

#include "core/types/battleDifficultyType.h"
#include "core/types/handType.h"
#include "core/types/turnType.h"

// Core speaks in enums; the UI displays strings. The mapping lives on the UI
// side of the boundary, so core never learns that these names exist.
// `inline` so every translation unit can include this without an ODR clash.

inline const char* turnTypeLabel(TurnType turnType) {
    return turnType == TurnType::Player ? "Player" : "Enemy";
}

inline const char* handTypeLabel(HandType hand) {
    return hand == HandType::Left ? "Left" : "Right";
}

inline const char* difficultyLabel(BattleDifficultyType difficulty) {
    switch (difficulty) {
    case BattleDifficultyType::Minor:
        return "Minor";
    case BattleDifficultyType::Major:
        return "Major";
    case BattleDifficultyType::Boss:
        return "Boss";
    }
    return "Unknown";
}
