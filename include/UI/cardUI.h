#pragma once

#include "engine/renderObject.h"
#include <cstdint>
#include "transformObject.h"
#include "renderObject.h"
#include "hitBoxObject.h"

class CardUI : public RenderObject {
public:
    explicit CardUI(uint32_t id) : _id(id) {}
    uint32_t id() const { return _id; }
    const TransformObject& getTransform() const;
    const RenderObject& getRender() const;
    const HitBoxObject& getHitBox() const;
private:
    uint32_t _id;
    TransformObject _transform;
    RenderObject _render;
    HitBoxObject _hitbox;
};