#include "core/systems/turnSystem.h"

void TurnSystem::advanceTurn() {
    changeWhoseTurn(_whoseTurn == TurnType::Player ? TurnType::Enemy : TurnType::Player);

    if (_whoseTurn == _startingTurn)
        incrementTurnCount();

    _turnAdvancedEventBus.emit(TurnAdvancedEvent{_whoseTurn, _turnCount});
}

void TurnSystem::incrementTurnCount() {
    _turnCount++;
}

void TurnSystem::changeWhoseTurn(TurnType whoseTurn) {
    _whoseTurn = whoseTurn;
}
