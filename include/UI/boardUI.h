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
    Vector2 tilePosition(HexCoord coord) const;
    Vector2 unitPosition(HexCoord coord) const;
    Vector2 tileSize() const {
        return {2.0f * _hexSize, sqrtf(3.0f) * _hexSize};
    }

    void addUnit(uint32_t unitId, HexCoord coord);
    void removeUnit(uint32_t unitId);
    void addConstruction(uint32_t constructionId, HexCoord coord);
    void removeConstruction(uint32_t unitId);

  private:
    std::vector<BoardTileUI> _tiles;
    float _hexSize;
};
