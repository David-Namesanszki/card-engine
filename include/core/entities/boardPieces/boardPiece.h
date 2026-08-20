#pragma once

#include <string>
#include "core/types/teamType.h"
#include "core/entities/entity.h"

enum class BoardPieceType {
    Unit,
    Construction
};

class BoardPiece : public Entity {
  public:
    BoardPiece(std::string name, TeamType team, BoardPieceType type)
        : _name(std::move(name)),
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

    virtual ~BoardPiece() = default;

  private:
    BoardPieceType _type;
    std::string _name;
    TeamType _team;
};