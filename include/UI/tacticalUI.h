#pragma once

#include <cstdint>
#include <list>
#include <string>
#include <vector>
#include "core/entities/boardTile.h"
#include "UI/boardUI.h"
#include "UI/configs/boardUIConfig.h"
#include "UI/panels/battleInfoPanelUI.h"
#include "UI/configs/battleInfoPanelUIConfig.h"
#include "UI/panels/progressPanelUI.h"
#include "UI/configs/progressPanelUIConfig.h"
#include "UI/rosterUI.h"
#include "UI/configs/rosterUIConfig.h"
#include "UI/unitUI.h"
#include "UI/configs/unitUIConfig.h"

// Composition for the tactical (deployment) view: the board with its units,
// framed by the battle info and progress panels. Units only get placed here —
// no combat happens during deployment, so there is nothing to damage or kill.
class TacticalUI {
  public:
    TacticalUI(
        BoardUIConfig boardConfig = {},
        const std::vector<BoardTile>& boardTiles = {},
        BattleInfoPanelUIConfig battleInfoPanelConfig = {},
        ProgressPanelUIConfig progressPanelConfig = {},
        RosterUIConfig rosterConfig = {},
        UnitUIConfig unitConfig = {}
    );

    void placeUnitOnBoard(uint32_t unitId, HexCoord coord);
    void removeUnitFromBoard(uint32_t unitId);

    void placeConstructionOnBoard(uint32_t constructionId, HexCoord coord);
    void removeConstructionFromBoard(uint32_t constructionId);

    const BoardUI& board() const {
        return _board;
    }
    const BattleInfoPanelUI& battleInfoPanel() const {
        return _battleInfoPanel;
    }
    BattleInfoPanelUI& battleInfoPanel() {
        return _battleInfoPanel;
    }
    const RosterUI& roster() const {
        return _roster;
    }
    const ProgressPanelUI& progressPanel() const {
        return _progressPanel;
    }
    const std::list<UnitUI>& units() const {
        return _units;
    }

  private:
    UnitUIConfig _unitConfig;
    BoardUI _board;
    BattleInfoPanelUI _battleInfoPanel;
    ProgressPanelUI _progressPanel;
    RosterUI _roster;
    std::list<UnitUI> _units;
};
