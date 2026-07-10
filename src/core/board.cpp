#include "core/board.h"

Board::Board(const std::vector<BoardTile>& boardTiles, HexCoord attackDirection)
    : _boardTiles(boardTiles),
      _attackDirection(attackDirection) {
}

const BoardTile* Board::tile(HexCoord coord) const {
    for (const auto& t : _boardTiles)
        if (t.coord() == coord)
            return &t;
    return nullptr;
}

BoardTile* Board::tile(HexCoord coord) {
    for (auto& t : _boardTiles)
        if (t.coord() == coord)
            return &t;
    return nullptr;
}

const BoardTile* Board::firstOccupiedTileAlong(HexCoord from, HexCoord dir) const {
    for (const BoardTile* t = tile(from + dir); t; t = tile(t->coord() + dir))
        if (t->isOccupied())
            return t;
    return nullptr;
}

std::optional<HexCoord> Board::find(uint32_t occupantId) const {
    for (const auto& t : _boardTiles)
        if (t.occupantId() == occupantId)
            return t.coord();
    return std::nullopt;
}

BoardResult Board::canPlace(HexCoord at, TeamType team, BoardTileType kind) const {
    const BoardTile* t = tile(at);
    if (!t)
        return BoardResult::err(BoardError::NoSuchTile);
    if (t->type() != kind)
        return BoardResult::err(BoardError::WrongTileType);
    if (t->team() != team)
        return BoardResult::err(BoardError::WrongTeam);
    if (t->isOccupied())
        return BoardResult::err(BoardError::TileOccupied);
    return BoardResult::ok();
}

BoardResult Board::place(HexCoord at, uint32_t id, TeamType team, BoardTileType kind) {
    BoardResult check = canPlace(at, team, kind);
    if (check.isErr())
        return check;
    tile(at)->setOccupant(id);
    return BoardResult::ok();
}

BoardResult Board::move(HexCoord from, HexCoord to, TeamType team, BoardTileType kind) {
    BoardTile* src = tile(from);
    if (!src)
        return BoardResult::err(BoardError::NoSuchTile);
    if (!src->isOccupied())
        return BoardResult::err(BoardError::NoOccupant);
    BoardResult check = canPlace(to, team, kind);
    if (check.isErr())
        return check;
    tile(to)->setOccupant(*src->occupantId());
    src->clearOccupant();
    return BoardResult::ok();
}

std::optional<uint32_t> Board::remove(HexCoord at) {
    BoardTile* t = tile(at);
    if (!t || !t->isOccupied())
        return std::nullopt;
    std::optional<uint32_t> removed = t->occupantId();
    t->clearOccupant();
    return removed;
}
