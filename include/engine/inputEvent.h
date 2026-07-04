#pragma once
#include "raylib.h"
#include <string>

enum class InputEventType {
    LeftClicked,
    LeftDown,
    LeftReleased,
    RightClicked,
    MouseMoved
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
    }
    return "Unknown";
}

struct InputEvent {
    InputEventType type;
    Vector2 position;
};