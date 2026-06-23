#pragma once

#include "engine/easing.h"
#include "engine/animator.h"
#include "UI/anchorPoint.h"
#include "UI/cardUI.h"
#include "raylib.h"
#include <functional>

using AnimFactory = std::function<Animation(CardUI&, const AnchorPoint&)>;
using EasingFn    = std::function<float(float)>;

struct LeftHandUIConfig {
    // RenderObject visual properties
    Vector2 position = {0.0f,   0.0f};
    Vector2 size     = {400.0f, 150.0f};
    Color   color    = {30, 30, 50, 200};
    int     layer    = 0;

    // Arc layout
    int     slotCount      = 5;
    Vector2 arcStartOffset = {-200.0f,   0.0f};
    Vector2 arcEndOffset   = { 200.0f,   0.0f};
    Vector2 controlOffset  = {   0.0f, -60.0f};

    // Animations
    AnimFactory addAnimDef = [](CardUI& card, const AnchorPoint& target) {
        Animation anim;
        anim.tag      = card.id();
        anim.duration = 2.0;
        anim.easing   = Easing::easeInOut;
        anim.channels = {
            {card.position.x, target.position.x, [&card](float v){ card.position.x = v; }},
            {card.position.y, target.position.y, [&card](float v){ card.position.y = v; }},
            {card.rotation,   target.rotation,   [&card](float v){ card.rotation   = v; }},
            {card.scale,      target.scale,       [&card](float v){ card.scale      = v; }},
        };
        return anim;
    };
    AnimFactory shiftAnimDef = [](CardUI& card, const AnchorPoint& target) {
        Animation anim;
        anim.tag      = card.id();
        anim.duration = 2.0;
        anim.easing   = Easing::easeInOut;
        anim.channels = {
            {card.position.x, target.position.x, [&card](float v){ card.position.x = v; }},
            {card.position.y, target.position.y, [&card](float v){ card.position.y = v; }},
            {card.rotation,   target.rotation,   [&card](float v){ card.rotation   = v; }},
            {card.scale,      target.scale,       [&card](float v){ card.scale      = v; }},
        };
        return anim;
    };
    AnimFactory removeAnimDef = [](CardUI& card, const AnchorPoint& target) {
        Animation anim;
        anim.tag      = card.id();
        anim.duration = 2.0;
        anim.easing   = Easing::easeInOut;
        anim.channels = {
            {card.position.x, target.position.x, [&card](float v){ card.position.x = v; }},
            {card.position.y, target.position.y, [&card](float v){ card.position.y = v; }},
            {card.rotation,   target.rotation,   [&card](float v){ card.rotation   = v; }},
            {card.scale,      target.scale,       [&card](float v){ card.scale      = v; }},
        };
        return anim;
    };
};
