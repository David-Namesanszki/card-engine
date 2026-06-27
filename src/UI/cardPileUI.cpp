#include "UI/cardPileUI.h"

CardPileUI::CardPileUI(CardPileUIConfig config)
    : sprite(config.sprite), transform(config.transform), hitbox(config.hitBox),
      _anchorPoint(config.drawPoint), _config(std::move(config)) {
    transform.addChild(&_anchorPoint.transform);
}

const AnchorPoint& CardPileUI::getAnchorPoint() const {
    return _anchorPoint;
}