#include "UI/cardUI.h"
#include "engine/easing.h"

void CardUI::animateTo(TransformComponent target, float duration) {
    _targetPos = target.position;
    _targetRot = target.rotation;
    _targetScale = target.scale;

    if (duration <= 0.0f) {
        // Snap immediately.
        transform.position = _targetPos;
        transform.rotation = _targetRot;
        transform.scale = _targetScale;
        _animating = false;
        return;
    }

    _startPos = transform.position;
    _startRot = transform.rotation;
    _startScale = transform.scale;
    _animDuration = duration;
    _animElapsed = 0.0f;
    _animating = true;
}

void CardUI::update(float dt) {
    if (!_animating)
        return;

    _animElapsed += dt;

    float t = _animElapsed / _animDuration;
    if (t >= 1.0f) {
        // Land exactly on the target and stop.
        transform.position = _targetPos;
        transform.rotation = _targetRot;
        transform.scale = _targetScale;
        _animating = false;
        return;
    }

    float e = Easing::easeInOut(t);
    transform.position.x = _startPos.x + (_targetPos.x - _startPos.x) * e;
    transform.position.y = _startPos.y + (_targetPos.y - _startPos.y) * e;
    transform.rotation = _startRot + (_targetRot - _startRot) * e;
    transform.scale = _startScale + (_targetScale - _startScale) * e;
}
