#include "core/board.h"

Board::Board(HexCoord attackDirection)
    : _attackDirection(attackDirection) {
}

void Board::addTile(HexCoord coord, BoardTileType type, TeamType team) {
    _boardTiles.emplace_back(coord, type, team);
}

std::vector<uint32_t> Board::getPlacedOccupantIds(BoardTileType type) {
    std::vector<uint32_t> occupant;
    for (BoardTile boardTile : _boardTiles) {
        if (boardTile.isOccupied() && boardTile.type() == type) {
            occupant.emplace_back(boardTile.occupantId());
        }
    }

    return occupant;
}

const BoardTile* Board::tile(HexCoord coord) const {
    auto it =
        std::find_if(_boardTiles.begin(), _boardTiles.end(), [coord](const BoardTile& boardTile) {
            return boardTile.coord() == coord;
        });

    return it == _boardTiles.end() ? nullptr : &*it;
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
