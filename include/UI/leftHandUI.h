#pragma once

#include "engine/renderObject.h"
#include "engine/animator.h"
#include "UI/configs/leftHandUIConfig.h"
#include "UI/anchorPoint.h"
#include "UI/cardUI.h"
#include <vector>
#include <list>
#include <functional>
#include <cstdint>

class LeftHandUI : public RenderObject {
public:
    LeftHandUI(Animator& animator, Vector2 position, LeftHandUIConfig config = {});

    void   addCard(CardUI card);
    CardUI removeCard(uint32_t id);

    const std::vector<AnchorPoint>& slots() const { return _slots; }
    const std::list<CardUI>&        cards() const { return _cards; }
    const AnchorPoint& getSlot0() const;

private:
    void    createSlots();
    float   slotT(int slot) const;
    Vector2 slotPosition(int slot) const;
    float   slotRotation(int slot) const;
    static void applyAnchor(CardUI& card, const AnchorPoint& anchor);
    std::list<CardUI>::iterator findCard(uint32_t id);

    Animator&                _animator;
    LeftHandUIConfig         _config;
    std::vector<AnchorPoint> _slots;
    std::list<CardUI>        _cards;
};
