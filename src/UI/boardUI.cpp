#include "UI/boardUI.h"
#include <algorithm>
#include "UI/boardTiles/effectBoardTileUI.h"
#include "core/entities/boardTiles/boardTile.h"
#include "UI/boardTiles/pieceBoardTileUI.h"
#include "core/entities/boardTiles/effectBoardTile.h"
#include "core/entities/boardTiles/pieceBoardTile.h"

BoardUI::BoardUI(
    uint32_t id,
    Vector2 position,
    float rotation,
    float scale,
    float hexSize,
    const std::vector<std::unique_ptr<BoardTile>>& tiles,
    const BoardUIConfig& config
)
    : EntityUI(id, position, rotation, scale),
      _hexSize(hexSize) {

    for (const std::unique_ptr<BoardTile>& data : tiles) {
        if (EffectBoardTile* effectTile = dynamic_cast<EffectBoardTile>(data.get())) {
            EffectBoardTileUI* tile = _tiles.emplace_back(
                EffectBoardTileUI(effectTile->getId(), effectTile->coord(), "")
            );
            tile->sprite.size = tileSize();
            tile->hitbox.setRectangle(tileSize());
            Vector2 hexPos = hexToPixelFlat(data->coord(), _hexSize);
            tile->transform.position = {
                transform.position.x + hexPos.x, transform.position.y + hexPos.y
            };
        } else if (PieceBoardTile* pieceTile = dynamic_cast<PieceBoardTile>(data.get())) {
            PieceBoardTileUI* tile = _tiles.emplace_back(PieceBoardTileUI(
                pieceTile->getId(), pieceTile->coord(), pieceTile->type(), pieceTile->team()
            ));
            tile->sprite.size = tileSize();
            tile->hitbox.setRectangle(tileSize());
            Vector2 hexPos = hexToPixelFlat(data->coord(), _hexSize);
            tile->transform.position = {
                transform.position.x + hexPos.x, transform.position.y + hexPos.y
            };
        }
    }

    std::stable_sort(_tiles.begin(), _tiles.end(), [](const BoardTileUI& a, const BoardTileUI& b) {
        return a.transform.position.y < b.transform.position.y;
    });

    for (auto& tile : _tiles) {
        tile->unitAnchor.transform.position = config.unitAnchorOffset;
        tile->transform.addChild(&tile->unitAnchor.transform);
    }
}

Vector2 BoardUI::tilePosition(HexCoord coord) const {
    Vector2 hexPos = hexToPixelFlat(coord, _hexSize);
    return {transform.position.x + hexPos.x, transform.position.y + hexPos.y};
}

Vector2 BoardUI::unitPosition(HexCoord coord) const {
}

void BoardUI::placeUnit(
    uint32_t unitId,
    HexCoord coord,
    std::string splashArt,
    std::string name,
    int currentHealth,
    int maxHealth,
    int attackPower,
    int defensePower,
    int armor,
    const UnitUIConfig& config
) {
    auto it = std::find_if(_tiles.begin(), _tiles.end(), [coord](std::unique_ptr<BoardTile> b) {
        return b->coord() == coord;
    });

    if (it == _tiles.end())
        throw "There is no board tile with this coord: {}";

    _pieces.emplace_back(
        UnitUI{
            unitId, coord, splashArt, name, currentHealth, attackPower, defensePower, armor, config
        }
    );
}

void BoardUI::placeConstruction(
    uint32_t unitId,
    HexCoord coord,
    std::string splashArt,
    std::string name,
    int durability,
    const ConstructionUIConfig& config
) {
    auto it = std::find_if(_tiles.begin(), _tiles.end(), [coord](std::unique_ptr<BoardTile> b) {
        return b->coord() == coord;
    });

    if (it == _tiles.end())
        throw "There is no board tile with this coord: {}";

    _pieces.emplace_back(ConstructionUI{unitId, coord, splashArt, name, durability, config});
}

void BoardUI::removeBoardPiece(uint32_t unitId) {
    auto it = std::find_if(_tiles.begin(), _tiles.end(), [unitId](BoardTileUI b) {
        if (b.occupant().has_value())
            return b.occupant().value() == unitId;
    });

    if (it == _tiles.end())
        throw "There is no unit with this id on this board.";

    _pieces.erase(
        std::find_if(_pieces.begin(), _pieces.end(), [unitId](std::unique_ptr<BoardPiece> b) {
            return b->getId() == unitId;
        })
    );
}