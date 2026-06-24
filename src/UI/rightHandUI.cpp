#include "UI/rightHandUI.h"
#include <algorithm>
#include <cmath>

RightHandUI::RightHandUI(Animator& animator, RightHandUIConfig config)
    : _animator(animator), _config(config) {
    transform.position = config.position;
    createSlots();
}

void RightHandUI::addCard(CardUI card) {
    int slot = 1;
    for (auto& c : _cards) {
        if (_config.shiftAnimDef && slot < _config.slotCount) {
            _animator.add(_config.shiftAnimDef(c, _slots[slot]));
        } else if (slot < _config.slotCount) {
            applyAnchor(c.transform, _slots[slot]);
        }
        slot++;
    }

    _cards.push_front(std::move(card));

    if (_config.addAnimDef) {
        _animator.add(_config.addAnimDef(_cards.front(), _slots[0]));
    } else {
        applyAnchor(_cards.front().transform, _slots[0]);
    }
}

CardUI RightHandUI::removeCard(uint32_t id) {
    auto it = findCard(id);
    int removedSlot = static_cast<int>(std::distance(_cards.begin(), it));

    // Kill any in-flight animation targeting this card before its storage dies.
    _animator.cancel(id);
    CardUI card = std::move(*it);
    _cards.erase(it);

    int slot = removedSlot;
    for (auto c = std::next(_cards.begin(), removedSlot); c != _cards.end(); ++c) {
        if (_config.shiftAnimDef) {
            _animator.add(_config.shiftAnimDef(*c, _slots[slot]));
        } else {
            applyAnchor(c->transform, _slots[slot]);
        }
        slot++;
    }

    return card;
}

const AnchorPoint& RightHandUI::getSlot0() const {
    return _slots[0];
}

#pragma region Helpers

void RightHandUI::applyAnchor(TransformComponent& t, const AnchorPoint& anchor) {
    t.position = anchor.transform.worldPosition();
    t.rotation = anchor.transform.rotation;
    t.scale    = anchor.transform.scale;
}

std::list<CardUI>::iterator RightHandUI::findCard(uint32_t id) {
    return std::find_if(_cards.begin(), _cards.end(),
        [id](const CardUI& c) { return c.id() == id; });
}

void RightHandUI::createSlots() {
    _slots.reserve(_config.slotCount);
    for (int i = 0; i < _config.slotCount; i++) {
        AnchorPoint slot;
        slot.transform.position = slotPosition(i);
        slot.transform.rotation = slotRotation(i);
        _slots.push_back(slot);
    }
}

float RightHandUI::slotT(int slot) const {
    if (_config.slotCount <= 1) return 0.5f;
    return static_cast<float>(slot) / static_cast<float>(_config.slotCount - 1);
}

Vector2 RightHandUI::slotPosition(int slot) const {
    float   t  = slotT(slot);
    float   u  = 1.0f - t;
    Vector2 p0 = {transform.position.x + _config.arcStartOffset.x, transform.position.y + _config.arcStartOffset.y};
    Vector2 p1 = {transform.position.x + _config.controlOffset.x,  transform.position.y + _config.controlOffset.y};
    Vector2 p2 = {transform.position.x + _config.arcEndOffset.x,   transform.position.y + _config.arcEndOffset.y};
    return {
        u*u*p0.x + 2.0f*u*t*p1.x + t*t*p2.x,
        u*u*p0.y + 2.0f*u*t*p1.y + t*t*p2.y
    };
}

float RightHandUI::slotRotation(int slot) const {
    float   t  = slotT(slot);
    float   u  = 1.0f - t;
    Vector2 p0 = {transform.position.x + _config.arcStartOffset.x, transform.position.y + _config.arcStartOffset.y};
    Vector2 p1 = {transform.position.x + _config.controlOffset.x,  transform.position.y + _config.controlOffset.y};
    Vector2 p2 = {transform.position.x + _config.arcEndOffset.x,   transform.position.y + _config.arcEndOffset.y};
    float dx = 2.0f*u*(p1.x - p0.x) + 2.0f*t*(p2.x - p1.x);
    float dy = 2.0f*u*(p1.y - p0.y) + 2.0f*t*(p2.y - p1.y);
    return atan2f(dy, dx) * RAD2DEG;
}

#pragma endregion
