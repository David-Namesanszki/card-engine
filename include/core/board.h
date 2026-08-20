#pragma once

#include <cstdint>
#include <optional>
#include <vector>
#include "core/boardError.h"
#include "core/entities/boardTiles/boardTile.h"
#include "core/entities/boardTiles/effectBoardTile.h"
#include "engine/hexCoord.h"
#include "engine/result.h"
#include <memory>
#include "core/entities/boardTiles/boardTile.h"
#include "core/entities/boardTiles/boardPieceBoardTile.h"

using BoardResult = Result<void, BoardError>;

class Board {
  public:
    Board(HexCoord attackDirection);

    void addEffectTile(HexCoord coord, TeamType team);
    void addBoardPieceBoardTile(HexCoord coord, BoardPieceType type, TeamType team);
    std::vector<uint32_t> getPlacedOccupantIds(BoardPieceType type);
    std::vector<uint32_t> getBoardPieceIds();

    HexCoord attackDirectionFor(TeamType team) const {
        return team == TeamType::Player ? _attackDirection : -_attackDirection;
    }
    const BoardTile* tile(HexCoord coord) const;
    std::optional<HexCoord> find(uint32_t occupantId) const;
    const BoardPieceBoardTile* firstOccupiedTileAlong(HexCoord from, HexCoord dir) const;

    bool canPlace(HexCoord at, TeamType team, BoardPieceType type) const;
    void place(HexCoord at, uint32_t id, TeamType team, BoardPieceType type);
    void move(HexCoord from, HexCoord to);
    std::optional<uint32_t> remove(HexCoord at);

  private:
    BoardTile* tile(HexCoord coord);

    std::vector<std::unique_ptr<BoardTile>> _boardTiles;
    HexCoord _attackDirection;
};
