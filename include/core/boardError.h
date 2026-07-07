#pragma once

// Why a placement or move is illegal. Board mutations return
// std::optional<BoardError>: nullopt means the action happened.
enum class BoardError {
    NoSuchTile,    // coordinate is not part of the board
    WrongTileType, // tile doesn't accept this kind of occupant (Effect tiles accept nothing)
    WrongTeam,     // tile belongs to a different team
    TileOccupied,
    NoOccupant,    // move source has nothing standing on it
};
