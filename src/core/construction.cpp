#include "core/entities/boardPieces/construction.h"

Construction::Construction(std::string name, TeamType team, Durability durability, HexCoord place)
    : BoardPiece(name, team, BoardPieceType::Construction, place),
      _durability(durability) {
}

bool Construction::isDestroyed() const {
    return _durability.isDestroyed();
}
