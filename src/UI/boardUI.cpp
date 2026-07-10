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

    // Anchors are linked after the sort: sorting copies tiles between slots,
    // which would break any transform parenting made earlier.
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
