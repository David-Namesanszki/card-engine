#pragma once

#include <cstddef>
#include <vector>
#include "core/entities/boardPieces/unit.h"
#include "core/entities/boardPieces/construction.h"
#include <unordered_map>
#include <memory>

class Roster {
  public:
    explicit Roster(size_t maxSize);

    void addUnit(std::string name, TeamType team, Health health, int attackPower, int defensePower);
    void addConstruction(std::string name, TeamType team, Durability durability);

    void removeBoardPiece(uint32_t id);

    const BoardPiece* getBoardPiece(uint32_t id) const;
    BoardPiece* getBoardPiece(uint32_t id);

  private:
    size_t _maxSize;
    std::vector<std::unique_ptr<BoardPiece>> _boardPieces;
};
