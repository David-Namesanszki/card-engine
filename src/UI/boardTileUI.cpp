#include "UI/boardTileUI.h"
#include <string>

namespace {
std::string getTexturePath(TeamType team, BoardTileType tileType, BoardPieceType pieceType) {
    switch (team) {
    case TeamType::Player:
        switch (pieceType) {
        case BoardPieceType::Unit:
            return "assets/boardTiles/unit_player.png";
        case BoardPieceType::Construction:
            return "assets/boardTiles/construction_player.png";
        }
        break;
    case TeamType::Enemy:
        switch (pieceType) {
        case BoardPieceType::Unit:
            return "assets/boardTiles/unit_enemy.png";
        case BoardPieceType::Construction:
            return "assets/boardTiles/construction_enemy.png";
        }
        break;
    case TeamType::Neutral:
        switch (tileType) {
        case BoardTileType::Effect:
            return "assets/boardTiles/effect.png";
        }
        break;
    }
    // Effect tiles are team-agnostic; also covers any unlisted type/team pairing.
    return "assets/boardTiles/effect.png";
}
} // namespace

BoardTileUI::BoardTileUI(
    HexCoord coord,
    BoardTileType tileType,
    BoardPieceType pieceType,
    TeamType team
)
    : _coord(coord) {
    sprite.texture = getTexturePath(team, tileType, pieceType);
    hitbox.setRectangle(sprite.size);
}
