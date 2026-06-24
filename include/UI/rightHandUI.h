#pragma once

#include "engine/animator.h"
#include "UI/configs/rightHandUIConfig.h"
#include "UI/anchorPoint.h"
#include "UI/cardUI.h"
#include <vector>
#include <list>
#include <cstdint>

class RightHandUI {
  public:
    RightHandUI(Animator& animator, RightHandUIConfig config = {});

    void addCard(CardUI card);
    CardUI removeCard(uint32_t id);

    const AnchorPoint& getSlot0() const;
    const std::list<CardUI>& cards() const { return _cards; }

    TransformComponent transform;
    SpriteComponent sprite;

  private:
    void createSlots();
    float slotT(int slot) const;
    Vector2 slotPosition(int slot) const;
    float slotRotation(int slot) const;
    static void applyAnchor(TransformComponent& transform, const AnchorPoint& anchor);
    std::list<CardUI>::iterator findCard(uint32_t id);

    Animator& _animator;
    RightHandUIConfig _config;
    std::vector<AnchorPoint> _slots;
    std::list<CardUI> _cards;
};
