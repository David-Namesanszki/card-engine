#include "renderer.h"
#include "battle.h"

static const Color COLOR_BG          = {30,  50,  30,  255};
static const Color COLOR_CARD_EMPTY  = {50,  50,  50,  255};
static const Color COLOR_CARD_FILLED = {70,  100, 140, 255};
static const Color COLOR_DRAW_PILE   = {60,  60,  60,  255};
static const Color COLOR_DISCARD     = {80,  30,  30,  255};

Renderer::Renderer() {}

void Renderer::drawBattle(const Battle& battle) {
    ClearBackground(COLOR_BG);

    // Draw pile — show remaining count
    drawPileSlot(_layout.drawPilePos, COLOR_DRAW_PILE, battle.getDrawPile().size());

    // Discard pile — show count
    drawPileSlot(_layout.discardPilePos, COLOR_DISCARD, battle.getDiscardPile().size());

    // Left hand
    const auto& leftCards = battle.getLeftHand().getCards();
    for (int i = 0; i < _layout.handSize; i++) {
        Vector2 pos = {_layout.leftHandPos.x + i * _layout.cardSpacing, _layout.leftHandPos.y};
        bool filled = i < static_cast<int>(leftCards.size());
        drawCardSlot(pos, filled ? COLOR_CARD_FILLED : COLOR_CARD_EMPTY);
    }

    // Right hand
    const auto& rightCards = battle.getRightHand().getCards();
    for (int i = 0; i < _layout.handSize; i++) {
        Vector2 pos = {_layout.rightHandPos.x + i * _layout.cardSpacing, _layout.rightHandPos.y};
        bool filled = i < static_cast<int>(rightCards.size());
        drawCardSlot(pos, filled ? COLOR_CARD_FILLED : COLOR_CARD_EMPTY);
    }
}

void Renderer::drawCardSlot(Vector2 pos, Color fill, const char* label) {
    DrawRectangleV(pos, {_layout.cardWidth, _layout.cardHeight}, fill);
    DrawRectangleLinesEx({pos.x, pos.y, _layout.cardWidth, _layout.cardHeight}, 1.0f, WHITE);
    if (label[0] != '\0')
        DrawText(label, pos.x + 5, pos.y + _layout.cardHeight / 2 - 8, 12, WHITE);
}

void Renderer::drawPileSlot(Vector2 pos, Color fill, int count) {
    drawCardSlot(pos, fill);
    DrawText(TextFormat("%d", count), pos.x + 5, pos.y + _layout.cardHeight / 2 - 8, 20, WHITE);
}
