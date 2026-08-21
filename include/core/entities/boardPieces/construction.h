#pragma once

#include <string>
#include <cstdint>
#include "core/attributes/durability.h"
#include "core/types/teamType.h"
#include "core/entities/boardPieces/boardPiece.h"
#include "engine/hexCoord.h"

class Construction : public BoardPiece {
  public:
    Construction(std::string name, TeamType team, Durability durability, HexCoord place);

    bool isDestroyed() const;

    const Durability& getDurability() const {
        return _durability;
    }

  private:
    Durability _durability;
};
