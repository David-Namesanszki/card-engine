#include "engine/systems/inputSystem.h"
#include "raylib.h"

std::vector<InputEvent> InputSystem::getInputs() {
    std::vector<InputEvent> events;
    Vector2 mouse = GetMousePosition();

    if (mouse.x != _lastMousePos.x || mouse.y != _lastMousePos.y) {
        events.push_back(InputEvent{InputEventType::MouseMoved, mouse});
        _lastMousePos = mouse;
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        events.push_back(InputEvent{InputEventType::LeftClicked, mouse});
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
        events.push_back(InputEvent{InputEventType::RightClicked, mouse});
    else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        events.push_back(InputEvent{InputEventType::LeftDown, mouse});
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        events.push_back(InputEvent{InputEventType::LeftReleased, mouse});

    float wheel = GetMouseWheelMove();
    if (wheel != 0.0f)
        events.push_back(InputEvent{InputEventType::Scrolled, mouse, wheel});

    return events;
}