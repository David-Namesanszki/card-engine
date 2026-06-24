#pragma once

#include "engine/easing.h"
#include "engine/animator.h"
#include "UI/anchorPoint.h"
#include "UI/cardUI.h"
#include "raylib.h"
#include <functional>

using AnimDef = std::function<Animation(TransformComponent&, const AnchorPoint&)>;

struct LeftHandUIConfig {
    Vector2 position = {0.0f, 0.0f};

    // Arc layout
    int slotCount = 5;
    Vector2 arcStartOffset = {-200.0f, 0.0f};
    Vector2 arcEndOffset = {200.0f, 0.0f};
    Vector2 controlOffset = {0.0f, -60.0f};

    // Animations
    AnimDef addAnimDef = [](TransformComponent& card, const AnchorPoint& target) {
        Vector2 wp = target.transform.worldPosition();
        Animation anim;
        anim.duration = 0.3f;
        anim.easing = Easing::easeInOut;
        anim.channels = {
            {card.position.x, wp.x, [&card](float v) { card.position.x = v; }},
            {card.position.y, wp.y, [&card](float v) { card.position.y = v; }},
            {card.rotation, target.transform.rotation, [&card](float v) { card.rotation = v; }},
            {card.scale, target.transform.scale, [&card](float v) { card.scale = v; }},
        };
        return anim;
    };

    AnimDef removeAnimDef = [](TransformComponent& card, const AnchorPoint& target) {
        Vector2 wp = target.transform.worldPosition();
        Animation anim;
        anim.duration = 0.3f;
        anim.easing = Easing::easeInOut;
        anim.channels = {
            {card.position.x, wp.x, [&card](float v) { card.position.x = v; }},
            {card.position.y, wp.y, [&card](float v) { card.position.y = v; }},
            {card.rotation, target.transform.rotation, [&card](float v) { card.rotation = v; }},
            {card.scale, target.transform.scale, [&card](float v) { card.scale = v; }},
        };
        return anim;
    };

    AnimDef shiftAnimDef = [](TransformComponent& card, const AnchorPoint& target) {
        Vector2 wp = target.transform.worldPosition();
        Animation anim;
        anim.duration = 0.2f;
        anim.easing = Easing::easeInOut;
        anim.channels = {
            {card.position.x, wp.x, [&card](float v) { card.position.x = v; }},
            {card.position.y, wp.y, [&card](float v) { card.position.y = v; }},
            {card.rotation, target.transform.rotation, [&card](float v) { card.rotation = v; }},
            {card.scale, target.transform.scale, [&card](float v) { card.scale = v; }},
        };
        return anim;
    };
};
