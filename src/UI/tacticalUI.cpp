#include "UI/tacticalUI.h"

TacticalUI::TacticalUI(
    BoardUIConfig boardConfig,
    const std::vector<BoardTile>& boardTiles,
    BattleInfoPanelUIConfig battleInfoPanelConfig,
    ProgressPanelUIConfig progressPanelConfig,
    RosterUIConfig rosterConfig,
    UnitUIConfig unitConfig
)
    : _unitConfig(unitConfig),
      _board(boardConfig, boardTiles),
      _battleInfoPanel(battleInfoPanelConfig),
      _progressPanel(progressPanelConfig),
      _roster(rosterConfig) {
    _units.emplace_back();
}

void TacticalUI::placeUnitOnBoard(uint32_t unitId, HexCoord coord) {
    _roster.removeUnit(unitId);
    _board.addUnit(unitId, coord);
}

void TacticalUI::placeConstructionOnBoard(uint32_t constructionId, HexCoord coord) {
    _roster.removeConstruction(constructionId);
    _board.addConstruction(constructionId, coord);
}

void TacticalUI::removeUnitFromBoard(uint32_t unitId) {
    _roster.addUnit(unitId);
    _board.removeUnit(unitId);
}

void TacticalUI::removeConstructionFromBoard(uint32_t constructionId) {
    _roster.addConstruction(constructionId);
    _board.removeConstruction(constructionId);
}