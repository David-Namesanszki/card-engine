#include "UI/cardPileUI.h"

CardPileUI::CardPileUI(CardPileUIConfig config, size_t size)
    : sprite(config.sprite),
      transform(config.transform),
      hitbox(config.hitBox),
      _anchorPoint(config.drawPoint),
      _config(std::move(config)) {
    transform.addChild(&_anchorPoint.transform);
    transform.addChild(&countText.transform);
    countText.transform.position = {-5.0f, -6.0f}; // roughly centred on the pile
    countText.text.text = std::to_string(size);    // later counts arrive via events
    hitbox.setRectangle(sprite.size);
}

void CardPileUI::setCount(int newCount) {
    countText.text.text = std::to_string(newCount);
}

const AnchorPoint& CardPileUI::getAnchorPoint() const {
    return _anchorPoint;
}