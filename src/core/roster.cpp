#include "core/roster.h"
#include <algorithm>

Roster::Roster(size_t maxSize)
    : _maxSize(maxSize) {
}

void Roster::addUnit(
    std::string name,
    TeamType team,
    Health health,
    int attackPower,
    int defensePower
) {
    if (_boardPieces.size() >= _maxSize)
        return;
    _boardPieces.push_back(
        std::make_unique<Unit>(std::move(name), team, health, attackPower, defensePower)
    );
}

void Roster::addConstruction(std::string name, TeamType team, Durability durability) {
    if (_boardPieces.size() >= _maxSize)
        return;
    _boardPieces.push_back(std::make_unique<Construction>(std::move(name), team, durability));
}

void Roster::removeBoardPiece(uint32_t id) {
    auto it = std::find_if(
        _boardPieces.begin(), _boardPieces.end(),
        [id](const std::unique_ptr<BoardPiece>& piece) { return piece->getId() == id; }
    );

    if (it != _boardPieces.end())
        _boardPieces.erase(it);
}

const BoardPiece* Roster::getBoardPiece(uint32_t id) const {
    auto it = std::find_if(
        _boardPieces.begin(), _boardPieces.end(), [id](const std::unique_ptr<BoardPiece>& piece) {
            return piece->getId() == id;
        }
    );
    return it == _boardPieces.end() ? nullptr : it->get();
}

BoardPiece* Roster::getBoardPiece(uint32_t id) {
    return const_cast<BoardPiece*>(static_cast<const Roster*>(this)->getBoardPiece(id));
}
