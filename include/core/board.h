#pragma once

#include <cstdint>
#include <optional>
#include <vector>
#include "core/boardError.h"
#include "core/boardTile.h"
#include "engine/hexCoord.h"
#include "engine/result.h"

using BoardResult = Result<void, BoardError>;

class Board {
  public:
    Board(const std::vector<BoardTile>& boardTiles, HexCoord attackDirection);

    const std::vector<BoardTile>& tiles() const {
        return _boardTiles;
    }
    HexCoord attackDirectionFor(TeamType team) const {
        return team == TeamType::Player ? _attackDirection : -_attackDirection;
    }
    const BoardTile* tile(HexCoord coord) const;
    std::optional<HexCoord> find(uint32_t occupantId) const;
    // Walks tile by tile from `from` (exclusive) along `dir` until the board
    // ends; returns the first occupied tile encountered, whatever its
    // occupant's team or kind.
    const BoardTile* firstOccupiedTileAlong(HexCoord from, HexCoord dir) const;

    BoardResult canPlace(HexCoord at, TeamType team, BoardTileType kind) const;
    BoardResult place(HexCoord at, uint32_t id, TeamType team, BoardTileType kind);
    BoardResult move(HexCoord from, HexCoord to, TeamType team, BoardTileType kind);
    std::optional<uint32_t> remove(HexCoord at);

  private:
    BoardTile* tile(HexCoord coord);

    std::vector<BoardTile> _boardTiles;
    HexCoord _attackDirection;
};
