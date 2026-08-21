#include "core/systems/effectSystem.h"

void EffectSystem::moveBoardPiece(uint32_t id, HexCoord to) {
    const BoardPiece* boardPiece = _boardPieces.at(id).get();
    const BoardTile& boardTile = _boardTiles.at(to);

    switch (boardTile.type()) {
    case BoardTileType::Effect:
        return;
    case BoardTileType::Unit:
        if (boardPiece->getType() != BoardPieceType::Unit)
            return;
    case BoardTileType::Construction:
        if (boardPiece->getType() != BoardPieceType::Construction)
            return;
    }
}

int EffectSystem::dealDamageToUnit(uint32_t id, int amount) {
    Unit* unit = dynamic_cast<Unit*>(_boardPieces.at(id).get());
    if (!unit)
        return 0;

    int excess = unit->takeDamage(amount);
    _unitDamagedEventBus.emit({id, unit->getHealth().current, unit->getArmor().getCurrentArmor()});
    if (!unit->isDead())
        return excess;

    _boardPieces.erase(id);
    _unitDiedEventBus.emit({id});

    return excess;
}

void EffectSystem::defendWithUnit(uint32_t id) {
    Unit* defender = dynamic_cast<Unit*>(_boardPieces.at(id).get());
    if (!defender)
        return;
    defender->defend();
    _unitDefendedEventBus.emit({id, defender->getArmor().getCurrentArmor()});
}

void EffectSystem::healUnit(uint32_t id, int amount) {
    Unit* healed = dynamic_cast<Unit*>(_boardPieces.at(id).get());
    if (!healed)
        return;
    healed->heal(amount);
    _unitHealedEventBus.emit({id, healed->getHealth()});
}

// void EffectSystem::attackWithUnit(uint32_t id) {
//     Unit* attacker = dynamic_cast<Unit*>(_boardPieces.at(id).get());
//     if (!attacker)
//         return;

//     HexCoord from = attacker->getPlace();

//     HexCoord dir = _board.attackDirectionFor(attacker->getTeam());
//     int damage = attacker->getAttackPower();

//     while (damage > 0) {
//         const BoardTile* targetTile = _board.firstOccupiedTileAlong(from, dir);
//         if (!targetTile)
//             return;

//         uint32_t targetId = _boardPieces;
//         Unit* target = dynamic_cast<Unit*>(_captain.getRoster().getBoardPiece(targetId));
//         if (!target || target->getTeam() == attacker->getTeam())
//             return; // the lane is blocked by a friendly or a non-unit occupant
//         from = targetTile->coord();
//         damage = dealDamageToUnit(targetId, damage);
//     }
// }