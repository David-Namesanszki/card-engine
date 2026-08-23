#pragma once

#include <string>
#include "core/types/teamType.h"
#include "core/entities/entity.h"
#include "engine/hexCoord.h"
#include "core/types/boardPieceType.h"

class BoardPiece : public Entity {
  public:
    BoardPiece(std::string name, TeamType team, BoardPieceType type, HexCoord place)
        : _name(std::move(name)),
          _place(place),
          _team(team),
          _type(type) {};

    const std::string& getName() const {
        return _name;
    }
    TeamType getTeam() const {
        return _team;
    }

    BoardPieceType getType() const {
        return _type;
    }

    HexCoord getPlace() const {
        return _place;
    }

    virtual ~BoardPiece() = default;

  private:
    HexCoord _place;
    BoardPieceType _type;
    std::string _name;
    TeamType _team;
};