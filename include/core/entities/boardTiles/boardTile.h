#pragma once

#include <cstdint>
#include <optional>
#include "core/types/teamType.h"
#include "engine/hexCoord.h"

enum class BoardTileType {
    Effect,
    BoardPiece,
};

class BoardTile {
  public:
    BoardTile(HexCoord coord, TeamType team, BoardTileType type)
        : _coord(coord),
          _team(team),
          _type(type) {
    }

    HexCoord coord() const {
        return _coord;
    }
    TeamType team() const {
        return _team;
    }

    virtual ~BoardTile() = default;

  private:
    BoardTileType _type;
    HexCoord _coord;
    TeamType _team;
};
