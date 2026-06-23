#include "raylib.h"
#include "engine/renderer.h"
#include "core/battle.h"
#include "UI/battleUI.h"
#include "engine/animator.h"

static const Color COLOR_SLOT      = {50,  50,  50,  255};
static const Color COLOR_CARD      = {70,  100, 140, 255};
static const Color COLOR_DRAW_PILE = {60,  60,  60,  255};
static const Color COLOR_DISCARD   = {80,  30,  30,  255};

int main() {
    InitWindow(1280, 720, "Card Engine");
    SetTargetFPS(60);

    CardPile deck;
    for (uint32_t i = 0; i < 14; i++)
        deck.addCard(i);

    Captain  captain;
    Battle   battle(captain, 0, deck);
    Animator animator;
    Renderer renderer;

    BattleUI battleUI({
        BattleUIConfig {},
        LeftHandUIConfig {},
        RightHandUIConfig {},
        DiscardPileUIConfig {},
        DrawPileUIConfig {},
    });


    battle.onCardDrawn([&](CardDrawnEvent e) {
        battleUI.drawCard(e.cardId);
    });

    battle.onCardDiscarded([&](CardDiscardedEvent e) {
        if(e.hand == HandType::Left)
            battleUI.discardFromLeftHand(e.cardId);

        if(e.hand == HandType::Right)
            battleUI.discardFromRightHand(e.cardId);
    });

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        animator.update(dt);

        if (IsKeyPressed(KEY_SPACE)) battle.startPlayerTurn();
        if (IsKeyPressed(KEY_E))     battle.endPlayerTurn();

        std::vector<const RenderObject*> cards;
        for (const auto& card : leftHand.cards())  cards.push_back(&card);
        for (const auto& card : rightHand.cards()) cards.push_back(&card);

        BeginDrawing();
        renderer.draw(cards);
        DrawText("SPACE - Start Turn    E - End Turn", 10, 10, 16, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
