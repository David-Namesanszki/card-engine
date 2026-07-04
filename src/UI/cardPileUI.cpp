#include "UI/cardPileUI.h"

CardPileUI::CardPileUI(CardPileUIConfig config)
    : sprite(config.sprite),
      transform(config.transform),
      hitbox(config.hitBox),
      _anchorPoint(config.drawPoint),
      _config(std::move(config)) {
    transform.addChild(&_anchorPoint.transform);
    hitbox.setRectangle(sprite.size);
}

void CardPileUI::setCount(int newCount) {
    countText.text.text = std::to_string(newCount);
}

const AnchorPoint& CardPileUI::getAnchorPoint() const {
    return _anchorPoint;
}