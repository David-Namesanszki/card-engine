#include "UI/boardTileUI.h"

BoardTileUI::BoardTileUI(uint32_t id, BoardTileType type, TeamType team)
    : _id(id),
      _type(type),
      _team(team) {
    _texturePath = getTexturePath();
    sprite.texture = _texturePath;
    hitbox.setRectangle(sprite.size);
}

std::string BoardTileUI::getTexturePath() const {
    switch (_team) {
    case TeamType::Player:
        switch (_type) {
        case BoardTileType::Unit:
            return "assets/boardTiles/unit_player.png";
        case BoardTileType::Construction:
            return "assets/boardTiles/construction_player.png";
        }
        break;
    case TeamType::Enemy:
        switch (_type) {
        case BoardTileType::Unit:
            return "assets/boardTiles/unit_enemy.png";
        case BoardTileType::Construction:
            return "assets/boardTiles/construction_enemy.png";
        }
        break;
    case TeamType::Neutral:
        switch (_type) {
        case BoardTileType::Effect:
            return "assets/boardTiles/effect.png";
        }
        break;
    }
    // Effect tiles are team-agnostic; also covers any unlisted type/team pairing.
    return "assets/boardTiles/effect.png";
}
