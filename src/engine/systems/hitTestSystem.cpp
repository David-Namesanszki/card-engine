#include "engine/systems/hitTestSystem.h"
#include <climits>

bool pointInPolygon(const Vector2& p, const std::vector<Vector2>& poly) {
    bool inside = false;
    size_t n = poly.size();
    for (size_t i = 0, j = n - 1; i < n; j = i++) {
        const Vector2& a = poly[i];
        const Vector2& b = poly[j];
        bool straddles = (a.y > p.y) != (b.y > p.y);
        if (straddles) {
            float xCross = a.x + (p.y - a.y) / (b.y - a.y) * (b.x - a.x);
            if (p.x < xCross)
                inside = !inside;
        }
    }
    return inside;
}

bool HitTestSystem::hit(
    const Vector2& testPosition,
    const TransformComponent& transform,
    const HitBoxComponent& hitBox
) const {
    if (hitBox.points.size() < 3)
        return false;

    Vector2 world = transform.worldPosition();
    Vector2 local = {testPosition.x - world.x, testPosition.y - world.y};

    float rad = -transform.worldRotation();
    float c = std::cos(rad);
    float s = std::sin(rad);
    Vector2 rotated = {local.x * c - local.y * s, local.x * s + local.y * c};

    float scale = transform.worldScale();
    if (scale != 0.0f) {
        rotated.x /= scale;
        rotated.y /= scale;
    }

    Vector2 query = {rotated.x - hitBox.position.x, rotated.y - hitBox.position.y};

    return pointInPolygon(query, hitBox.points);
}

std::optional<HitResult> HitTestSystem::hitTest(
    const Vector2& testPosition,
    const BattleUI& battle
) const {

    std::optional<HitResult> best;
    // int bestLayer = INT_MIN;

    // auto consider = [&](EntityType type,
    //                     const TransformComponent& t,
    //                     const HitBoxComponent& hb,
    //                     uint32_t cardId = 0) {
    //     if (hit(testPosition, t, hb) && hb.layer >= bestLayer) {
    //         bestLayer = hb.layer;
    //         best = HitResult{type, cardId};
    //     }
    // };

    // consider(EntityType::DrawPile, battle.drawPile().transform, battle.drawPile().hitbox);
    // consider(EntityType::DiscardPile, battle.discardPile().transform,
    // battle.discardPile().hitbox); consider(EntityType::LeftHand, battle.leftHand().transform,
    // battle.leftHand().hitbox); consider(EntityType::RightHand, battle.rightHand().transform,
    // battle.rightHand().hitbox); for (const auto& card : battle.cards())
    //     consider(EntityType::Card, card.transform, card.hitbox, card.id());

    // consider(EntityType::Button, battle.endTurnButton().transform,
    // battle.endTurnButton().hitbox);

    return best;
}
