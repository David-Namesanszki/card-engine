#pragma once

#include "UI/boardTiles/boardTileUI.h"
#include "UI/panels/pip.h"
#include "UI/textUI.h"

class EffectBoardTileUI : public BoardTileUI {
  public:
    EffectBoardTileUI(uint32_t id, HexCoord coord, std::string description);

    TextUI effectDescriptionText;
    Pip effectDescriptionPip;

  private:
    std::string _description;
};