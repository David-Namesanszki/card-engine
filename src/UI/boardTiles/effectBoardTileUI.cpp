#include "UI/boardTiles/effectBoardTileUI.h"

EffectBoardTileUI::EffectBoardTileUI(uint32_t id, HexCoord coord, std::string description)
    : BoardTileUI(id, coord),
      _description(std::move(description)) {
}