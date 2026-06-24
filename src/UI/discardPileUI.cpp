#include "UI/discardPileUI.h"

DiscardPileUI::DiscardPileUI(Animator& animator, DiscardPileUIConfig config)
    : sprite(config.sprite), transform(config.transform), hitbox(config.hitBox),
      discardPoint(config.discardPoint), _animator(animator), _config(std::move(config)) {
    transform.addChild(&discardPoint.transform);
}

void DiscardPileUI::add(CardUI card) {
    auto it = _cards.insert(_cards.end(), std::move(card));
    CardUI& landed = *it;

    if (_config.receiveAnimDef) {
        Animation anim = _config.receiveAnimDef(landed.transform, discardPoint);
        anim.tag = landed.id();
        _animator.add(std::move(anim));
    } else {
        landed.transform.position = discardPoint.transform.worldPosition();
        landed.transform.rotation = discardPoint.transform.rotation;
        landed.transform.scale    = discardPoint.transform.scale;
    }
}

CardUI DiscardPileUI::removeTopCard() {
    // Kill any in-flight receive animation before its storage dies.
    _animator.cancel(_cards.back().id());
    CardUI card = std::move(_cards.back());
    _cards.pop_back();
    return card;
}
