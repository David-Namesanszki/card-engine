#pragma once

#include <cstdint>
#include "engine/components/transformComponent.h"

// Base for anything the UI refers to by id. Owns identity and placement only;
// what it looks like is the derived type's business.
class EntityUI {
  public:
    explicit EntityUI(
        uint32_t id,
        Vector2 position = {0.0f, 0.0f},
        float rotation = 0.0f,
        float scale = 1.0f
    )
        : _id(id) {
        transform.position = position;
        transform.rotation = rotation;
        transform.scale = scale;
    }

    // Polymorphic base: copying would slice, and copying a TransformComponent
    // would unlink the original's children.
    EntityUI(const EntityUI&) = delete;
    EntityUI& operator=(const EntityUI&) = delete;
    virtual ~EntityUI() = default;

    uint32_t id() const {
        return _id;
    }

    TransformComponent transform;

  private:
    uint32_t _id;
};
