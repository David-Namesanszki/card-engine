#include "engine/systems/intentSystem.h"

void IntentSystem::changeState(const InputState& newState) {
    _state = newState;
}

const InputState& IntentSystem::getInputState() const {
    return _state;
}

IntentType IntentSystem::interpret(const InputEventType& input, const EntityType& entity) const {
    if (input == InputEventType::LeftClicked && entity == EntityType::Card) {
        if (_state == InputState::Normal)
            return IntentType::TryPlayCard;
        if (_state == InputState::Target)
            return IntentType::SelectTarget;
    } else if (input == InputEventType::LeftClicked && entity == EntityType::Button) {
        // A button's meaning never depends on interaction state.
        return IntentType::EndTurn;
    } else if (input == InputEventType::RightClicked && _state == InputState::Target)
        return IntentType::CancelCardPlay;
    return IntentType::None;
}