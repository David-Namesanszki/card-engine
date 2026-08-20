#pragma once

#include <cstdint>
#include "core/entities/boardTiles/boardTile.h"
#include "engine/components/transformComponent.h"
#include "engine/components/spriteComponent.h"
#include "engine/components/hitBoxComponent.h"
#include "UI/anchorPoint.h"
#include "core/entities/boardPieces/boardPiece.h"

class BoardTileUI {
  public:
    BoardTileUI(HexCoord coord, BoardTileType tileType, BoardPieceType pieceType, TeamType team);

    // A tile is a location, so the hex coordinate is its identity — the same
    // vocabulary the core Board speaks. There is no separate tile id.
    HexCoord coord() const {
        return _coord;
    }

    TransformComponent transform;
    SpriteComponent sprite;
    HitBoxComponent hitbox;
    // Where a unit standing on this tile is placed (see BoardUI::unitPosition).
    AnchorPoint unitAnchor;

    void addOccupant(uint32_t id) {
        _occupant.emplace(id);
    }

    std::optional<uint32_t> occupant() const {
        return _occupant;
    }

  private:
    std::optional<uint32_t> _occupant = std::nullopt;
    HexCoord _coord;
};
