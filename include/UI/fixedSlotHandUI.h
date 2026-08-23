#pragma once

#include "engine/systems/animationSystem.h"
#include "UI/configs/handUIConfig.h"
#include "UI/anchorPoint.h"
#include "UI/cardUI.h"
#include <vector>
#include <list>
#include <cstdint>

class FixedSlotHandUI {
  public:
    FixedSlotHandUI(HandUIConfig config = {});

    const AnchorPoint& getPosition(size_t index) const;
    size_t slotCount() const {
        return _slots.size();
    }

    TransformComponent transform;
    SpriteComponent sprite;
    HitBoxComponent hitbox;

  private:
    float slotT(int slot) const;
    Vector2 slotPosition(int slot) const;
    float slotRotation(int slot) const;
    void reorganizeHand();

    HandUIConfig _config;
    std::vector<AnchorPoint> _slots;
};
