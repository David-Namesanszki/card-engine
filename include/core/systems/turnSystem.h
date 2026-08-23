#pragma once

#include "engine/eventBus.h"
#include "core/events/turnAdvancedEvent.h"
#include "core/types/turnType.h"

class TurnSystem {
  public:
    TurnSystem(TurnType whoseTurn)
        : _whoseTurn(whoseTurn),
          _startingTurn(whoseTurn),
          _turnCount(0) {
    }

    void advanceTurn();

    void onTurnAdvanced(std::function<void(TurnAdvancedEvent)> cb) {
        _turnAdvancedEventBus.subscribe(cb);
    }

  private:
    void incrementTurnCount();
    void changeWhoseTurn(TurnType whoseTurn);

    TurnType _whoseTurn;
    TurnType _startingTurn;
    size_t _turnCount;

    EventBus<TurnAdvancedEvent> _turnAdvancedEventBus;
};
