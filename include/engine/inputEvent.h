#pragma once
#include "raylib.h"
#include <string>

enum class InputEventType {
    LeftClicked,
    LeftDown,
    LeftReleased,
    RightClicked,
    MouseMoved,
    Scrolled
};

inline std::string to_string(InputEventType type) {
    switch (type) {
    case InputEventType::LeftClicked:
        return "LeftClicked";
    case InputEventType::LeftDown:
        return "LeftDown";
    case InputEventType::LeftReleased:
        return "LeftReleased";
    case InputEventType::RightClicked:
        return "RightClicked";
    case InputEventType::MouseMoved:
        return "MouseMoved";
    case InputEventType::Scrolled:
        return "Scrolled";
    }
    return "Unknown";
}

struct InputEvent {
    InputEventType type;
    Vector2 position;
    float scrollDelta = 0.0f; // wheel notches; meaningful only for Scrolled
};