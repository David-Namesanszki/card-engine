#pragma once
#include <vector>
#include "raylib.h"
#include "engine/inputEvent.h"

class InputSystem {
  public:
    std::vector<InputEvent> getInputs();

  private:
    Vector2 _lastMousePos = {-1.0f, -1.0f};
};
