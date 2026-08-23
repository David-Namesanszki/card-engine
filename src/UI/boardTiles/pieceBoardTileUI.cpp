#include "UI/boardTiles/pieceBoardTileUI.h"

PieceBoardTileUI::PieceBoardTileUI(uint32_t id, HexCoord coord, BoardPieceType type, TeamType team)
    : BoardTileUI(id, coord) {
    sprite.texture = getTexturePath(team, type);
}

static std::string getTexturePath(TeamType team, BoardPieceType type) {
    switch (team) {
    case TeamType::Player:
        switch (type) {
        case BoardPieceType::Unit:
            return "assets/boardTiles/unit_player.png";
        case BoardPieceType::Construction:
            return "assets/boardTiles/construction_player.png";
        }
        break;
    case TeamType::Enemy:
        switch (type) {
        case BoardPieceType::Unit:
            return "assets/boardTiles/unit_enemy.png";
        case BoardPieceType::Construction:
            return "assets/boardTiles/construction_enemy.png";
        }
        break;
        // Effect tiles are team-agnostic; also covers any unlisted type/team pairing.
        return "assets/boardTiles/effect.png";
    }
    return "";
}