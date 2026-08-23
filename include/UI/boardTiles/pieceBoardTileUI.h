#pragma once

#include "UI/boardTiles/boardTileUI.h"

class PieceBoardTileUI : public BoardTileUI {
  public:
    PieceBoardTileUI(uint32_t id, HexCoord coord, BoardPieceType type, TeamType team);
};