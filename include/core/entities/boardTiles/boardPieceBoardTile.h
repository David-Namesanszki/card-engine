#pragma once

#include "core/entities/boardTiles/boardTile.h"
#include <memory>
#include "core/entities/boardPieces/boardPiece.h"

class BoardPieceBoardTile : public BoardTile {
  public:
    BoardPieceBoardTile(HexCoord hexCoord, TeamType teamType, BoardPieceType boardPieceType)
        : BoardTile(hexCoord, teamType, BoardTileType::BoardPiece),
          _boardPieceType(boardPieceType) {
    }

    bool isOccupied() const {
        return _boardPieceId.has_value();
    }
    std::optional<uint32_t> getBoardPieceId() const {
        return _boardPieceId;
    }

    void setBoardPiece(uint32_t id) {
        _boardPieceId = id;
    }
    void clearBoardPiece() {
        _boardPieceId.reset();
    }

    BoardPieceType getBoardPieceType() const {
        return _boardPieceType;
    }

  private:
    std::optional<uint32_t> _boardPieceId;
    BoardPieceType _boardPieceType;
};