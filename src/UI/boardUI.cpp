#include "UI/boardUI.h"
#include <algorithm>

BoardUI::BoardUI(const BoardUIConfig& config, const std::vector<BoardTile>& tiles)
    : sprite(config.sprite),
      transform(config.transform),
      _hexSize(config.hexSize) {
    _tiles.reserve(tiles.size());

    for (const auto& data : tiles) {
        _tiles.emplace_back(data.coord(), data.type(), data.team());
        BoardTileUI& tile = _tiles.back();
        // Sized from hexSize so the tiles stay seamless at any grid size.
        tile.sprite.size = tileSize();
        tile.hitbox.setRectangle(tileSize());
        Vector2 hexPos = hexToPixelFlat(data.coord(), _hexSize);
        tile.transform.position = {
            transform.position.x + hexPos.x, transform.position.y + hexPos.y
        };
    }

    std::stable_sort(_tiles.begin(), _tiles.end(), [](const BoardTileUI& a, const BoardTileUI& b) {
        return a.transform.position.y < b.transform.position.y;
    });

    for (auto& tile : _tiles) {
        tile.unitAnchor.transform.position = config.unitAnchorOffset;
        tile.transform.addChild(&tile.unitAnchor.transform);
    }
}

Vector2 BoardUI::tilePosition(HexCoord coord) const {
    Vector2 hexPos = hexToPixelFlat(coord, _hexSize);
    return {transform.position.x + hexPos.x, transform.position.y + hexPos.y};
}

Vector2 BoardUI::unitPosition(HexCoord coord) const {
    for (const auto& tile : _tiles)
        if (tile.coord() == coord)
            return tile.unitAnchor.transform.worldPosition();
    return tilePosition(coord);
}

void BoardUI::addUnit(uint32_t unitId, HexCoord coord) {
    auto it =
        std::find_if(_tiles.begin(), _tiles.end(), [coord](BoardTileUI b) { b.coord() == coord; });

    if (it == _tiles.end())
        throw "There is no board tile with this coord: {}";

    it->addOccupant(unitId);
}

void BoardUI::addConstruction(uint32_t constructionId, HexCoord coord) {
    auto it =
        std::find_if(_tiles.begin(), _tiles.end(), [coord](BoardTileUI b) { b.coord() == coord; });

    if (it == _tiles.end())
        throw "There is no board tile with this coord: {}";

    it->addOccupant(constructionId);
}

void BoardUI::removeUnit(uint32_t unitId) {
    auto it = std::find_if(_tiles.begin(), _tiles.end(), [unitId](BoardTileUI b) {
        if (b.occupant().has_value())
            return b.occupant().value() == unitId;
    });

    if (it == _tiles.end())
        throw "There is no unit with this id on this board.";

    it->occupant().reset();
}

void BoardUI::removeConstruction(uint32_t constructionId) {
    auto it = std::find_if(_tiles.begin(), _tiles.end(), [constructionId](BoardTileUI b) {
        if (b.occupant().has_value())
            return b.occupant().value() == constructionId;
    });

    if (it == _tiles.end())
        throw "There is no construction with this id on this board.";

    it->occupant().reset();
}