#pragma once

#include "engine/inputEvent.h"
#include "engine/systems/hitTestSystem.h"

enum class IntentType {
    None,
    TryPlayCard,
    SelectTarget,
    EndTurn,
    CancelCardPlay
};

enum class InputState {
    Normal,
    Target
};

class IntentSystem {
  public:
    void changeState(const InputState& newState);
    const InputState& getInputState() const;

    IntentType interpret(const InputEventType& input, const EntityType& entity) const;

  private:
    InputState _state;
};