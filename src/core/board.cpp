#include "core/board.h"

Board::Board(HexCoord attackDirection)
    : _attackDirection(attackDirection) {
}

void Board::addEffectTile(HexCoord coord, TeamType team) {
    _boardTiles.push_back(std::make_unique<EffectBoardTile>(coord, team));
}

void Board::addBoardPieceBoardTile(HexCoord coord, BoardPieceType type, TeamType team) {
    _boardTiles.push_back(std::make_unique<BoardPieceBoardTile>(coord, team, type));
}

std::vector<uint32_t> Board::getBoardPieceIds() {
    std::vector<uint32_t> boardPieces;
    for (const auto& boardTile : _boardTiles) {
        auto* pieceTile = dynamic_cast<BoardPieceBoardTile*>(boardTile.get());
        if (!pieceTile)
            continue;

        if (pieceTile->isOccupied())
            boardPieces.push_back(*pieceTile->getBoardPieceId());
    }

    return boardPieces;
}

std::vector<uint32_t> Board::getPlacedOccupantIds(BoardPieceType type) {
    std::vector<uint32_t> occupant;
    for (const auto& boardTile : _boardTiles) {
        auto* pieceTile = dynamic_cast<BoardPieceBoardTile*>(boardTile.get());
        if (!pieceTile)
            continue;

        if (pieceTile->isOccupied() && pieceTile->getBoardPieceType() == type)
            occupant.push_back(*pieceTile->getBoardPieceId());
    }

    return occupant;
}

const BoardTile* Board::tile(HexCoord coord) const {
    auto it = std::find_if(
        _boardTiles.begin(),
        _boardTiles.end(),
        [coord](const std::unique_ptr<BoardTile>& boardTile) { return boardTile->coord() == coord; }
    );

    return it == _boardTiles.end() ? nullptr : it->get();
}

BoardTile* Board::tile(HexCoord coord) {
    return const_cast<BoardTile*>(static_cast<const Board*>(this)->tile(coord));
}

const BoardPieceBoardTile* Board::firstOccupiedTileAlong(HexCoord from, HexCoord dir) const {
    for (const BoardTile* t = tile(from + dir); t; t = tile(t->coord() + dir)) {
        auto* pieceTile = dynamic_cast<const BoardPieceBoardTile*>(t);
        if (!pieceTile)
            continue;

        if (pieceTile->isOccupied())
            return pieceTile;
    }
    return nullptr;
}

std::optional<HexCoord> Board::find(uint32_t occupantId) const {
    for (const auto& t : _boardTiles) {
        auto* pieceTile = dynamic_cast<const BoardPieceBoardTile*>(t.get());
        if (!pieceTile)
            continue;

        if (pieceTile->getBoardPieceId() == occupantId)
            return pieceTile->coord();
    }

    return std::nullopt;
}

bool Board::canPlace(HexCoord at, TeamType team, BoardPieceType type) const {
    const BoardPieceBoardTile* pieceTile = dynamic_cast<const BoardPieceBoardTile*>(tile(at));

    if (!pieceTile || pieceTile->getBoardPieceType() != type || pieceTile->team() != team ||
        pieceTile->isOccupied())
        return false;

    return true;
}

void Board::place(HexCoord at, uint32_t id, TeamType team, BoardPieceType type) {
    if (canPlace(at, team, type)) {
        auto* pieceTile = dynamic_cast<BoardPieceBoardTile*>(tile(at));
        if (!pieceTile)
            return;
        pieceTile->setBoardPiece(id);
    }
}

void Board::move(HexCoord from, HexCoord to) {
    auto* srcPieceTile = dynamic_cast<BoardPieceBoardTile*>(tile(from));
    if (!srcPieceTile)
        return;

    auto* targetPieceTile = dynamic_cast<BoardPieceBoardTile*>(tile(to));
    if (!targetPieceTile)
        return;

    if (canPlace(to, srcPieceTile->team(), srcPieceTile->getBoardPieceType())) {
        if (srcPieceTile->isOccupied()) {
            targetPieceTile->setBoardPiece(*srcPieceTile->getBoardPieceId());
            srcPieceTile->clearBoardPiece();
        }
    }
}

std::optional<uint32_t> Board::remove(HexCoord at) {
    BoardPieceBoardTile* pieceTile = dynamic_cast<BoardPieceBoardTile*>(tile(at));
    if (!pieceTile || !pieceTile->isOccupied())
        return std::nullopt;

    std::optional<uint32_t> removed = pieceTile->getBoardPieceId();
    pieceTile->clearBoardPiece();

    return removed;
}
