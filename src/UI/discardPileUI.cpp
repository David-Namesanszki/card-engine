#include "UI/discardPileUI.h"

DiscardPileUI::DiscardPileUI(const int count, const DiscardPileUIConfig& config)
    : _count(count), _config(config)
{}

void DiscardPileUI::setCount(const int newCount) {
    _count = newCount;
}

AnchorPoint DiscardPileUI::getDiscardPoint() const {
    AnchorPoint result = _config.discardPoint;
    result.position.x += position.x;
    result.position.y += position.y;
    return result;
}