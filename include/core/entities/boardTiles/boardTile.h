#pragma once

#include <cstdint>
#include <optional>
#include "core/types/teamType.h"
#include "engine/hexCoord.h"
#include "core/entities/entity.h"

class BoardTile : public Entity {
  public:
    BoardTile(HexCoord coord)
        : _coord(coord) {
    }

    HexCoord coord() const {
        return _coord;
    }

    virtual ~BoardTile() = default;

  private:
    HexCoord _coord;
};
