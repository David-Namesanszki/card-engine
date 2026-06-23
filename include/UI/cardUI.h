#pragma once

#include "engine/renderObject.h"
#include <cstdint>

class CardUI : public RenderObject {
public:
    explicit CardUI(uint32_t id) : _id(id) {}
    uint32_t id() const { return _id; }
private:
    uint32_t _id;
};