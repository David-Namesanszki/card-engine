#pragma once

#include <cstdint>
#include <optional>
#include "core/types/teamType.h"
#include "engine/hexCoord.h"

enum class BoardTileType {
    Unit,
    Construction,
    Effect,
};

class BoardTile {
  public:
    BoardTile(HexCoord coord, BoardTileType type, TeamType team)
        : _coord(coord),
          _type(type),
          _team(team) {
    }

    HexCoord coord() const {
        return _coord;
    }
    BoardTileType type() const {
        return _type;
    }
    TeamType team() const {
        return _team;
    }
    bool isOccupied() const {
        return _occupantId.has_value();
    }
    std::optional<uint32_t> occupantId() const {
        return _occupantId;
    }

    void setOccupant(uint32_t id) {
        _occupantId = id;
    }
    void clearOccupant() {
        _occupantId.reset();
    }

  private:
    HexCoord _coord;
    BoardTileType _type;
    TeamType _team;
    std::optional<uint32_t> _occupantId;
};
