#pragma once

#include "engine/animator.h"
#include "engine/easing.h"
#include "engine/hitBoxComponent.h"
#include "engine/spriteComponent.h"
#include "engine/transformComponent.h"
#include "UI/anchorPoint.h"
#include <functional>

struct DiscardPileUIConfig {
    HitBoxComponent hitBox;
    SpriteComponent sprite;
    TransformComponent transform;
    AnchorPoint discardPoint = {};

    std::function<Animation(TransformComponent&, const AnchorPoint&)> receiveAnimDef =
        [](TransformComponent& card, const AnchorPoint& target) {
            Vector2 wp = target.transform.worldPosition();
            Animation anim;
            anim.duration = 0.3f;
            anim.easing   = Easing::easeIn;
            anim.channels = {
                {card.position.x, wp.x,                      [&card](float v){ card.position.x = v; }},
                {card.position.y, wp.y,                      [&card](float v){ card.position.y = v; }},
                {card.rotation,   target.transform.rotation, [&card](float v){ card.rotation   = v; }},
                {card.scale,      0.5f,                      [&card](float v){ card.scale      = v; }},
            };
            return anim;
        };
};