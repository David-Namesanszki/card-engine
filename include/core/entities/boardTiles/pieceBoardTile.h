#pragma once

#include "core/types/teamType.h"
#include "engine/hexCoord.h"
#include "core/types/boardPieceType.h"
#include "core/entities/boardTiles/boardTile.h"

class PieceBoardTile : public BoardTile {
  public:
    PieceBoardTile(HexCoord coord, TeamType team, BoardPieceType type)
        : BoardTile(coord),
          _team(team),
          _type(type) {
    }

    TeamType team() const {
        return _team;
    }
    BoardPieceType type() const {
        return _type;
    }

  private:
    TeamType _team;
    BoardPieceType _type;
};