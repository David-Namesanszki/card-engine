#include "engine/animator.h"
#include <algorithm>

void Animator::add(Animation anim) {
    if (anim.tag != 0)
        _animations.erase(
            std::remove_if(_animations.begin(), _animations.end(),
                [&](const Animation& a) { return a.tag == anim.tag; }),
            _animations.end()
        );
    _animations.push_back(std::move(anim));
}

void Animator::cancel(uint32_t tag) {
    if (tag == 0)
        return;
    _animations.erase(
        std::remove_if(_animations.begin(), _animations.end(),
            [tag](const Animation& a) { return a.tag == tag; }),
        _animations.end()
    );
}

void Animator::update(float dt) {
    for (auto& anim : _animations) {
        if (anim.delay > 0.0f) { anim.delay -= dt; continue; }
        anim.elapsed += dt;
        float easedT = anim.easing(std::clamp(anim.elapsed / anim.duration, 0.0f, 1.0f));
        for (auto& ch : anim.channels)
            ch.setter(ch.from + (ch.to - ch.from) * easedT);
    }

    for (auto& anim : _animations)
        if (anim.delay <= 0.0f && anim.elapsed >= anim.duration && anim.onComplete)
            anim.onComplete();

    _animations.erase(
        std::remove_if(_animations.begin(), _animations.end(),
            [](const Animation& a) { return a.delay <= 0.0f && a.elapsed >= a.duration; }),
        _animations.end()
    );
}
