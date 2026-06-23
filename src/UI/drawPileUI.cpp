#include "UI/drawPileUI.h"

DrawPileUI::DrawPileUI(const int count, const DrawPileUIConfig& config)
    : _count(count), _config(config)
{}

void DrawPileUI::setCount(const int newCount) {
    _count = newCount;
}

AnchorPoint DrawPileUI::getDrawPoint() const {
    AnchorPoint result = _config.drawPoint;
    result.position.x += position.x;
    result.position.y += position.y;
    return result;
}