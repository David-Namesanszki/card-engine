#pragma once

#include <string>
#include "engine/components/hitBoxComponent.h"
#include "engine/components/spriteComponent.h"
#include "engine/hexCoord.h"
#include "UI/configs/boardPieceUIConfig.h"
#include "UI/entityUI.h"
#include "UI/panels/pip.h"
#include "UI/textUI.h"

// A piece standing on a board tile. Owns everything a unit and a construction
// have in common: the portrait, its hitbox, the tile it occupies, and the name
// banner. Derived types add their own readouts.
class BoardPieceUI : public EntityUI {
  public:
    BoardPieceUI(
        uint32_t id,
        HexCoord coord,
        std::string splashArt,
        std::string name,
        const BoardPieceUIConfig& config
    );

    ~BoardPieceUI() override = default;

    HexCoord coord() const {
        return _coord;
    }
    void setCoord(HexCoord coord) {
        _coord = coord;
    }

    void setName(const std::string& name);

    SpriteComponent sprite;
    HitBoxComponent hitbox;

    TextUI nameText;
    Pip namePip;

  private:
    HexCoord _coord;
    Vector2 _nameCenter; // banner centre, for centring variable-length names
};
