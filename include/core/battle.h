#pragma once

#include "core/types/battleDifficultyType.h"
#include "core/types/turnType.h"
#include "core/board.h"
#include <vector>
#include "core/entities/boardPieces/boardPiece.h"
#include <memory>

class Battle {
  public:
    std::vector<std::unique_ptr<BoardPiece>> boardPieces;
    std::vector<BoardTile> boardTiles;
    BattleDifficultyType difficulty;
    TurnType whoseTurn;
};