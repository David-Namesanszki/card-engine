#pragma once
#include <cstdint>
#include <optional>
#include "raylib.h"
#include "engine/components/hitBoxComponent.h"
#include "engine/components/transformComponent.h"

class BattleUI;

enum class EntityType { Card, DrawPile, DiscardPile, LeftHand, RightHand, Button };

struct HitResult {
    EntityType type;
    uint32_t cardId; // meaningful only when type == EntityType::Card
};

class HitTestSystem {
  public:
    // Returns the topmost hit entity (highest layer; ties broken by consideration
    // order, so later/topmost wins), or nullopt if nothing is hit.
    std::optional<HitResult> hitTest(const Vector2& testPosition, const BattleUI& battle) const;

  private:
    bool hit(
        const Vector2& testPosition,
        const TransformComponent& transform,
        const HitBoxComponent& hitBox
    ) const;
};
