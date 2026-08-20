#pragma once

#include "core/entities/boardTiles/boardTile.h"

class EffectBoardTile : public BoardTile {
  public:
    EffectBoardTile(HexCoord hexCoord, TeamType teamType)
        : BoardTile(hexCoord, teamType, BoardTileType::Effect) {};

  private:
};