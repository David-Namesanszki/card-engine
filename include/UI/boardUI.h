#pragma once

#include <vector>
#include "UI/boardTileUI.h"
#include "engine/components/hexComponent.h"
#include "engine/components/transformComponent.h"
#include "engine/components/spriteComponent.h"
#include "UI/configs/boardUIConfig.h"

class BoardUI {
  public:
    BoardUI(const BoardUIConfig& config);

    SpriteComponent sprite;
    TransformComponent transform;

    const std::vector<BoardTileUI>& tiles() const {
        return _tiles;
    }

  private:
    std::vector<BoardTileUI> _tiles;
};
