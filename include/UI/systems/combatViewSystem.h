#pragma once

#include "UI/captainUI.h"
#include "UI/boardPieces/boardPieceUI.h"
#include <memory>
#include "UI/boardPieces/boardTileUI.h"

class CombatViewSystem {
  public:
  private:
    CaptainUI _captain;
    std::vector<std::unique_ptr<BoardPieceUI>> _boardPieces;
    std::vector<std::unique_ptr<BoardTileUI>> _tiles;
};