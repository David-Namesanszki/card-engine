#pragma once

#include <cstdint>
#include <string>
#include "UI/boardPieces/boardPieceUI.h"
#include "UI/configs/constructionUIConfig.h"
#include "UI/panels/pip.h"
#include "UI/textUI.h"

class ConstructionUI : public BoardPieceUI {
  public:
    ConstructionUI(
        uint32_t id,
        HexCoord coord,
        std::string splashArt,
        std::string name,
        int durability,
        const ConstructionUIConfig& config = {}
    );

    void setDurability(int durability);

    TextUI durabilityText;
    Pip durabilityPip;
};
