#pragma once

#include "core/board.h"
#include "core/deck.h"
#include "core/attributes/actionPoints.h"
#include "core/battleInfo.h"
#include "core/attributes/health.h"
#include "core/unit.h"
#include "core/construction.h"

struct BattleStartedEvent {
    Health captainHealth;
    Deck deck;
    Board board;
    std::vector<Unit> units;
    std::vector<Construction> constructions;
    ActionPoints actionPoints;
    BattleInfo battleInfo;
};