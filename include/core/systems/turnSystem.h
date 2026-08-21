#pragma once

#include "engine/eventBus.h"
#include "core/events/whoseTurnChangedEvent.h"
#include "core/events/turnCountIncrementedEvent.h"
#include "core/types/turnType.h"

class TurnSystem {
  public:
    TurnSystem(TurnType whoseTurn)
        : _whoseTurn(whoseTurn),
          _turnCount(0) {
    }
    void incrementTurnCount();
    void changeWhoseTurn();

    void onTurnCountIncremented(std::function<void(TurnCountIncrementedEvent)> cb) {
        _turnCountIncrementedEventBus.subscribe(cb);
    }
    void onWhoseTurnchanged(std::function<void(WhoseTurnChangedEvent)> cb) {
        _whoseTurnChangedEventBus.subscribe(cb);
    }

  private:
    TurnType _whoseTurn;
    size_t _turnCount;

    EventBus<TurnCountIncrementedEvent> _turnCountIncrementedEventBus;
    EventBus<WhoseTurnChangedEvent> _whoseTurnChangedEventBus;
};