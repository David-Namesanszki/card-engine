#pragma once

#include <vector>
#include "UI/boardPieces/boardTileUI.h"
#include "engine/components/hexComponent.h"
#include "engine/components/transformComponent.h"
#include "engine/components/spriteComponent.h"
#include "UI/configs/boardUIConfig.h"
#include "UI/configs/constructionUIConfig.h"
#include "UI/boardPieces/boardPieceUI.h"
#include "UI/boardPieces/unitUI.h"
#include "UI/boardPieces/constructionUI.h"
#include <memory>
#include "UI/entityUI.h"

class BoardUI : public EntityUI {
  public:
    BoardUI(
        uint32_t id,
        Vector2 position,
        float rotation,
        float scale,
        float hexSize = 100.0f,
        const std::vector<std::unique_ptr<BoardTile>>& tiles,
        const BoardUIConfig& config
    );

    Vector2 tilePosition(HexCoord coord) const;
    Vector2 unitPosition(HexCoord coord) const;
    Vector2 tileSize() const {
        return {2.0f * _hexSize, sqrtf(3.0f) * _hexSize};
    }

    void placeUnit(
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
    );
    void placeConstruction(
        uint32_t unitId,
        HexCoord coord,
        std::string splashArt,
        std::string name,
        int durability,
        const ConstructionUIConfig& config
    );
    void removeBoardPiece(uint32_t unitId);

  private:
    std::vector<std::unique_ptr<BoardPieceUI>> _pieces;
    std::vector<std::unique_ptr<BoardTileUI>> _tiles;
    float _hexSize;
};
