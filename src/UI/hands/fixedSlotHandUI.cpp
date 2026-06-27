#include "UI/hands/fixedSlotHandUI.h"
#include <algorithm>
#include <cmath>
#include <iterator>

FixedSlotHandUI::FixedSlotHandUI(HandUIConfig config) : _config(config) {
    transform = config.transform;

    _slots.resize(_config.slotCount);
    for (int i = 0; i < _config.slotCount; i++) {
        AnchorPoint slot;
        slot.transform.position = slotPosition(i);
        slot.transform.rotation = slotRotation(i);
        _slots[i] = slot;
    }
}

const std::list<uint32_t>& FixedSlotHandUI::getCardIds() const {
    return _cardIds;
}

void FixedSlotHandUI::addCardId(uint32_t cardId, size_t index) {
    _cardIds.insert(std::next(_cardIds.begin(), index), cardId);
}

void FixedSlotHandUI::removeCardId(uint32_t cardId) {
    std::find(_cardIds.begin(), _cardIds.end(), [cardId](uint32_t card) { return card == cardId; });
}

const AnchorPoint& FixedSlotHandUI::getPosition(size_t index) const {
    return _slots[index];
}

float FixedSlotHandUI::slotT(int slot) const {
    if (_config.slotCount <= 1)
        return 0.5f;
    return static_cast<float>(slot) / static_cast<float>(_config.slotCount - 1);
}

Vector2 FixedSlotHandUI::slotPosition(int slot) const {
    float t = slotT(slot);
    float u = 1.0f - t;
    Vector2 p0 = {
        transform.position.x + _config.arcStartOffset.x,
        transform.position.y + _config.arcStartOffset.y
    };
    Vector2 p1 = {
        transform.position.x + _config.controlOffset.x,
        transform.position.y + _config.controlOffset.y
    };
    Vector2 p2 = {
        transform.position.x + _config.arcEndOffset.x, transform.position.y + _config.arcEndOffset.y
    };
    return {
        u * u * p0.x + 2.0f * u * t * p1.x + t * t * p2.x,
        u * u * p0.y + 2.0f * u * t * p1.y + t * t * p2.y
    };
}

float FixedSlotHandUI::slotRotation(int slot) const {
    float t = slotT(slot);
    float u = 1.0f - t;
    Vector2 p0 = {
        transform.position.x + _config.arcStartOffset.x,
        transform.position.y + _config.arcStartOffset.y
    };
    Vector2 p1 = {
        transform.position.x + _config.controlOffset.x,
        transform.position.y + _config.controlOffset.y
    };
    Vector2 p2 = {
        transform.position.x + _config.arcEndOffset.x, transform.position.y + _config.arcEndOffset.y
    };
    float dx = 2.0f * u * (p1.x - p0.x) + 2.0f * t * (p2.x - p1.x);
    float dy = 2.0f * u * (p1.y - p0.y) + 2.0f * t * (p2.y - p1.y);
    return atan2f(dy, dx); // radians, matching TransformComponent's convention
};
