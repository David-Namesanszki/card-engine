#pragma once

#include <vector>
#include "UI/boardTileUI.h"
#include "engine/components/hexComponent.h"
#include "engine/components/transformComponent.h"
#include "engine/components/spriteComponent.h"
#include "UI/configs/boardUIConfig.h"

class BoardUI {
  public:
    // config is the look; tiles are the board's structure (identity), sourced
    // from the core board at the composition root.
    BoardUI(const BoardUIConfig& config, const std::vector<BoardTile>& tiles = {});

    SpriteComponent sprite;
    TransformComponent transform;

    const std::vector<BoardTileUI>& tiles() const {
        return _tiles;
    }
    // Screen-space centre of the tile at a core hex coordinate.
    Vector2 tilePosition(HexCoord coord) const;
    // Screen-space point where a unit on that tile stands (the tile's unit
    // anchor); falls back to the tile centre for coords with no tile.
    Vector2 unitPosition(HexCoord coord) const;
    // On-screen size of one tile: a flat-top hex of radius s is 2s wide by
    // sqrt(3)*s tall. Also sizes whatever stands on a tile.
    Vector2 tileSize() const {
        return {2.0f * _hexSize, sqrtf(3.0f) * _hexSize};
    }

  private:
    std::vector<BoardTileUI> _tiles;
    float _hexSize;
};
