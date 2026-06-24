#pragma once

#include "engine/easing.h"
#include "engine/animator.h"
#include "UI/anchorPoint.h"
#include "UI/cardUI.h"
#include "raylib.h"
#include <functional>

using AnimFactory = std::function<Animation(CardUI&, const AnchorPoint&)>;

struct RightHandUIConfig {
    Vector2 position = {0.0f, 0.0f};

    // Arc layout
    int     slotCount      = 5;
    Vector2 arcStartOffset = {-200.0f,   0.0f};
    Vector2 arcEndOffset   = { 200.0f,   0.0f};
    Vector2 controlOffset  = {   0.0f, -60.0f};

    // Animations
    AnimFactory addAnimDef = [](CardUI& card, const AnchorPoint& target) {
        Vector2 wp = target.transform.worldPosition();
        Animation anim;
        anim.tag      = card.id();
        anim.duration = 0.3f;
        anim.easing   = Easing::easeInOut;
        anim.channels = {
            {card.transform.position.x, wp.x,                      [&card](float v) { card.transform.position.x = v; }},
            {card.transform.position.y, wp.y,                      [&card](float v) { card.transform.position.y = v; }},
            {card.transform.rotation,   target.transform.rotation, [&card](float v) { card.transform.rotation   = v; }},
            {card.transform.scale,      target.transform.scale,    [&card](float v) { card.transform.scale      = v; }},
        };
        return anim;
    };

    AnimFactory shiftAnimDef = [](CardUI& card, const AnchorPoint& target) {
        Vector2 wp = target.transform.worldPosition();
        Animation anim;
        anim.tag      = card.id();
        anim.duration = 0.2f;
        anim.easing   = Easing::easeInOut;
        anim.channels = {
            {card.transform.position.x, wp.x,                      [&card](float v) { card.transform.position.x = v; }},
            {card.transform.position.y, wp.y,                      [&card](float v) { card.transform.position.y = v; }},
            {card.transform.rotation,   target.transform.rotation, [&card](float v) { card.transform.rotation   = v; }},
            {card.transform.scale,      target.transform.scale,    [&card](float v) { card.transform.scale      = v; }},
        };
        return anim;
    };
};
