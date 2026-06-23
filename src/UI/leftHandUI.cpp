#include "UI/leftHandUI.h"
#include <algorithm>
#include <cmath>

LeftHandUI::LeftHandUI(Animator& animator, Vector2 pos, LeftHandUIConfig config)
    : _animator(animator), _config(config) {
    position = pos;
    createSlots();
}

void LeftHandUI::addCard(const CardUI& card) {
    _transform.addObject(&card.getTransform());
    _render.addObject(&card.getRender());
    _hitbox.addObject(&card.getHitBox());

    int slot = 1;
    for (auto& c : _cards) {
        if (_config.shiftAnimDef && slot < _config.slotCount)  {
            _animator.add(_config.shiftAnimDef(c, _slots[slot]));
        }
        else if(slot < _config.slotCount){
            applyAnchor(c, _slots[slot]);
        }
        slot++;
    }

    _cards.push_front(std::move(card));
    _cards.front().parent = this;
    children.push_back(&_cards.front());

    if (_config.addAnimDef) {
        _animator.add(_config.addAnimDef(_cards.front(), _slots[0]));
    }
    else {
        applyAnchor(_cards.front(), _slots[0]);
    }
}

CardUI LeftHandUI::removeCard(uint32_t id) {
    auto it          = findCard(id);
    int  removedSlot = static_cast<int>(std::distance(_cards.begin(), it));

    auto childIt = std::find(children.begin(), children.end(), &*it);
    if (childIt != children.end()) children.erase(childIt);

    CardUI card = std::move(*it);
    card.parent = nullptr;
    _cards.erase(it);

    int slot = removedSlot;
    for (auto c = std::next(_cards.begin(), removedSlot); c != _cards.end(); ++c) {
        if (_config.shiftAnimDef) {
            _animator.add(_config.shiftAnimDef(*c, _slots[slot]));
        }
        else {
            applyAnchor(*c, _slots[slot]);
        }
        slot++;
    }

    return card;
}

const AnchorPoint& LeftHandUI::getSlot0() const {
    return _slots[0];
}

#pragma region Helpers

void LeftHandUI::applyAnchor(CardUI& card, const AnchorPoint& anchor) {
    card.position = anchor.position;
    card.rotation = anchor.rotation;
    card.scale    = anchor.scale;
}

std::list<CardUI>::iterator LeftHandUI::findCard(uint32_t id) {
    return std::find_if(_cards.begin(), _cards.end(),
        [id](const CardUI& c){ return c.id() == id; });
}

void LeftHandUI::createSlots() {
    _slots.reserve(_config.slotCount);
    for (int i = 0; i < _config.slotCount; i++) {
        AnchorPoint slot;
        slot.position = slotPosition(i);
        slot.rotation = slotRotation(i);
        _slots.push_back(slot);
    }
}

float LeftHandUI::slotT(int slot) const {
    if (_config.slotCount <= 1) return 0.5f;
    return static_cast<float>(slot) / static_cast<float>(_config.slotCount - 1);
}

Vector2 LeftHandUI::slotPosition(int slot) const {
    float   t  = slotT(slot);
    float   u  = 1.0f - t;
    Vector2 p0 = {_config.arcStartOffset.x, _config.arcStartOffset.y};
    Vector2 p1 = {_config.controlOffset.x,  _config.controlOffset.y};
    Vector2 p2 = {_config.arcEndOffset.x,   _config.arcEndOffset.y};
    return {
        u*u*p0.x + 2.0f*u*t*p1.x + t*t*p2.x,
        u*u*p0.y + 2.0f*u*t*p1.y + t*t*p2.y
    };
}

float LeftHandUI::slotRotation(int slot) const {
    float   t  = slotT(slot);
    float   u  = 1.0f - t;
    Vector2 p0 = {_config.arcStartOffset.x, _config.arcStartOffset.y};
    Vector2 p1 = {_config.controlOffset.x,  _config.controlOffset.y};
    Vector2 p2 = {_config.arcEndOffset.x,   _config.arcEndOffset.y};
    float dx = 2.0f*u*(p1.x - p0.x) + 2.0f*t*(p2.x - p1.x);
    float dy = 2.0f*u*(p1.y - p0.y) + 2.0f*t*(p2.y - p1.y);
    return atan2f(dy, dx) * RAD2DEG;
}

#pragma endregion
