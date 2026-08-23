#pragma once

#include <cstdint>
#include "engine/components/transformComponent.h"
#include "engine/components/spriteComponent.h"
#include "engine/components/hitBoxComponent.h"
#include "UI/anchorPoint.h"
#include "core/entities/boardPieces/boardPiece.h"
#include "UI/entityUI.h"

class BoardTileUI : public EntityUI {
  public:
    BoardTileUI(uint32_t id, HexCoord coord)
        : EntityUI(id),
          _coord(coord) {};

    // A tile is a location, so the hex coordinate is its identity — the same
    // vocabulary the core Board speaks. There is no separate tile id.
    HexCoord coord() const {
        return _coord;
    }

    SpriteComponent sprite;
    HitBoxComponent hitbox;
    // Where a unit standing on this tile is placed (see BoardUI::unitPosition).
    AnchorPoint unitAnchor;

    virtual ~BoardTileUI() = default;

  private:
    HexCoord _coord;
};
