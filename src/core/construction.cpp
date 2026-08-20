#include "core/entities/boardPieces/construction.h"

Construction::Construction(std::string name, TeamType team, Durability durability)
    : BoardPiece(name, team, BoardPieceType::Construction),
      _durability(durability) {
}

bool Construction::isDestroyed() const {
    return _durability.isDestroyed();
}
