#pragma once

#include <unordered_map>
#include "engine/hexCoord.h"
#include <memory>
#include "core/entities/boardPieces/boardPiece.h"
#include "core/entities/boardTile.h"
#include "core/entities/boardPieces/unit.h"
#include "core/events/unitDamagedEvent.h"
#include "core/events/unitHealedEvent.h"
#include "core/events/unitDiedEvent.h"
#include "core/events/unitDefendedEvent.h"
#include "engine/eventBus.h"

class EffectSystem {
  public:
    EffectSystem(
        std::vector<std::unique_ptr<BoardPiece>> boardPieces,
        std::vector<BoardTile> boardTiles
    );
    void moveBoardPiece(uint32_t id, HexCoord to);
    int dealDamageToUnit(uint32_t id, int amount);
    void defendWithUnit(uint32_t id);
    void healUnit(uint32_t id, int amount);
    // void attackWithUnit(uint32_t id);

    void onUnitDamaged(std::function<void(UnitDamagedEvent)> cb) {
        _unitDamagedEventBus.subscribe(cb);
    }
    void onUnitHealed(std::function<void(UnitHealedEvent)> cb) {
        _unitHealedEventBus.subscribe(cb);
    }
    void onUnitDied(std::function<void(UnitDiedEvent)> cb) {
        _unitDiedEventBus.subscribe(cb);
    }
    void onUnitDefended(std::function<void(UnitDefendedEvent)> cb) {
        _unitDefendedEventBus.subscribe(cb);
    }

  private:
    std::unordered_map<uint32_t, std::unique_ptr<BoardPiece>> _boardPieces;
    std::unordered_map<HexCoord, BoardTile> _boardTiles;

    EventBus<UnitDamagedEvent> _unitDamagedEventBus;
    EventBus<UnitDiedEvent> _unitDiedEventBus;
    EventBus<UnitHealedEvent> _unitHealedEventBus;
    EventBus<UnitDefendedEvent> _unitDefendedEventBus;
};