#pragma once

#include "core/board.h"
#include "core/roster.h"
#include "core/deck.h"
#include "core/attributes/actionPoints.h"
#include "core/battleInfo.h"
#include "core/attributes/health.h"

struct TacticalPhaseStartedEvent {
    Health captainHealth;
    Roster roster;
    Deck deck;
    Board board;
    int actionPoints;
    int firePoints;
    BattleInfo battleInfo;
};