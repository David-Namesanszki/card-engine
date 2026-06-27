#pragma once

#include "engine/animator.h"
#include "UI/configs/handUIConfig.h"
#include "UI/anchorPoint.h"
#include "UI/cardUI.h"
#include <vector>
#include <list>
#include <cstdint>

class FixedSlotHandUI {
  public:
    FixedSlotHandUI(HandUIConfig config = {});

    const std::list<uint32_t>& getCardIds() const;
    void addCardId(uint32_t cardId, size_t index);
    void removeCardId(uint32_t cardId);
    const AnchorPoint& getPosition(size_t index) const;

    TransformComponent transform;
    SpriteComponent sprite;

  private:
    float slotT(int slot) const;
    Vector2 slotPosition(int slot) const;
    float slotRotation(int slot) const;

    HandUIConfig _config;
    std::list<uint32_t> _cardIds;
    std::vector<AnchorPoint> _slots;
};
