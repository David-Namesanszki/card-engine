#pragma once

#include "UI/configs/scrollableGridUIConfig.h"
#include "UI/configs/unitUIConfig.h"
#include "UI/configs/constructionUIConfig.h"

// Pure theme for the roster strip. Which entries exist is game content and
// reaches RosterUI as addUnitEntry/addConstructionEntry calls sourced from
// the captain's roster. Entries render as full unit/construction cards, so
// they share the same per-card theme used on the battlefield.
struct RosterUIConfig {
    ScrollableGridUIConfig grid; // viewport chrome, entry pitch, wrap/scroll axis

    UnitUIConfig unitConfig;
    ConstructionUIConfig constructionConfig;
};
