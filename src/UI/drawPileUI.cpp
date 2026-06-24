#include "UI/drawPileUI.h"

DrawPileUI::DrawPileUI(DrawPileUIConfig config)
    : sprite(config.sprite), transform(config.transform), hitbox(config.hitBox),
      drawPoint(config.drawPoint), _config(std::move(config)) {
    transform.addChild(&drawPoint.transform);
}

void DrawPileUI::add(CardUI card) {
    card.transform.position = drawPoint.transform.worldPosition();
    card.transform.rotation = drawPoint.transform.rotation;
    card.transform.scale    = drawPoint.transform.scale;
    _cards.push_back(std::move(card));
}

CardUI DrawPileUI::removeTopCard() {
    CardUI card = std::move(_cards.back());
    _cards.pop_back();
    return card;
}
