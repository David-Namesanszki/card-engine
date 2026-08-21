#include "core/systems/turnSystem.h"

void TurnSystem::incrementTurnCount() {
    _turnCount++;
    _turnCountIncrementedEventBus.emit(TurnCountIncrementedEvent{_turnCount});
}
void TurnSystem::changeWhoseTurn() {
    if (_whoseTurn == TurnType::Enemy)
        _whoseTurn = TurnType::Player;
    else
        _whoseTurn = TurnType::Enemy;

    _whoseTurnChangedEventBus.emit(WhoseTurnChangedEvent{_whoseTurn});
}