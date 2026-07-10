#include "UI/boardTileUI.h"
#include <string>

namespace {
std::string getTexturePath(TeamType team, BoardTileType type) {
    switch (team) {
    case TeamType::Player:
        switch (type) {
        case BoardTileType::Unit:
            return "assets/boardTiles/unit_player.png";
        case BoardTileType::Construction:
            return "assets/boardTiles/construction_player.png";
        }
        break;
    case TeamType::Enemy:
        switch (type) {
        case BoardTileType::Unit:
            return "assets/boardTiles/unit_enemy.png";
        case BoardTileType::Construction:
            return "assets/boardTiles/construction_enemy.png";
        }
        break;
    case TeamType::Neutral:
        switch (type) {
        case BoardTileType::Effect:
            return "assets/boardTiles/effect.png";
        }
        break;
    }
    // Effect tiles are team-agnostic; also covers any unlisted type/team pairing.
    return "assets/boardTiles/effect.png";
}
} // namespace

BoardTileUI::BoardTileUI(HexCoord coord, BoardTileType type, TeamType team)
    : _coord(coord) {
    sprite.texture = getTexturePath(team, type);
    hitbox.setRectangle(sprite.size);
}
