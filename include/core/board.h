#pragma once

#include <cstdint>
#include <optional>
#include <vector>
#include "core/boardError.h"
#include "core/boardTile.h"
#include "engine/hexCoord.h"
#include "engine/result.h"

// "It happened, or here is why it didn't" — success carries no payload.
using BoardResult = Result<void, BoardError>;

// The battle board: hex tiles with fixed slot rules and changing occupancy.
// Tiles hold only occupant ids — the Unit objects live in Battle, which
// passes the occupant's team/kind into each mutation so the board can enforce
// the spatial rules: matching tile type and team, no stacking, and nothing on
// Effect tiles.
class Board {
  public:
    Board(const std::vector<BoardTile>& boardTiles);

    const std::vector<BoardTile>& tiles() const {
        return _boardTiles;
    }
    const BoardTile* tile(HexCoord coord) const;
    std::optional<HexCoord> find(uint32_t occupantId) const;

    BoardResult canPlace(HexCoord at, TeamType team, BoardTileType kind) const;
    BoardResult place(HexCoord at, uint32_t id, TeamType team, BoardTileType kind);
    BoardResult move(HexCoord from, HexCoord to, TeamType team, BoardTileType kind);
    std::optional<uint32_t> remove(HexCoord at);

  private:
    BoardTile* tileAt(HexCoord coord);

    std::vector<BoardTile> _boardTiles;
};
