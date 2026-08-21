#pragma once

#include <cstdint>
#include <optional>
#include <vector>
#include "core/boardError.h"
#include "engine/hexCoord.h"
#include "engine/result.h"
#include <memory>
#include "core/entities/boardTile.h"

class Board {
  public:
    Board(HexCoord attackDirection)
        : _attackDirection(attackDirection) {};

    void addBoardTile(BoardTile boardTile) {
        _boardTiles.emplace_back(std::move(boardTile));
    }

    HexCoord attackDirectionFor(TeamType team) const {
        return team == TeamType::Player ? _attackDirection : -_attackDirection;
    }

    std::vector<BoardTile> getBoardTiles() const {
        return _boardTiles;
    }

  private:
    std::vector<BoardTile> _boardTiles;
    HexCoord _attackDirection;
};
