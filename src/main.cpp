#include "raylib.h"
#include "UI/leftHandUI.h"
#include "UI/drawPileUI.h"
#include "UI/discardPileUI.h"
#include "engine/animator.h"
#include "engine/easing.h"
#include <algorithm>
#include <list>
#include <random>
#include <vector>

static void drawCard(const CardUI& card, Color col) {
    float   scale = card.transform.scale;
    float   w     = 60.0f * scale;
    float   h     = 90.0f * scale;
    Vector2 pos   = card.transform.position;
    DrawRectanglePro({pos.x, pos.y, w, h}, {w / 2, h / 2}, card.transform.rotation, col);
    DrawText(TextFormat("%d", card.id()), (int)pos.x - 6, (int)pos.y - 8, 16, WHITE);
}

static void drawSlotGhosts(const LeftHandUI& hand) {
    for (const auto& slot : hand.slots()) {
        Vector2 wp = slot.transform.worldPosition();
        DrawRectanglePro({wp.x, wp.y, 60, 90}, {30, 45}, slot.transform.rotation, {255, 255, 255, 25});
    }
}

static void drawPileBack(Vector2 pos, int count, Color col, const char* label) {
    DrawRectangle((int)pos.x - 30, (int)pos.y - 45, 60, 90, col);
    DrawText(TextFormat("%d", count), (int)pos.x - 8, (int)pos.y - 8, 18, WHITE);
    DrawText(label, (int)pos.x - 22, (int)pos.y + 52, 12, LIGHTGRAY);
}

int main() {
    InitWindow(1280, 720, "Card Flow Demo");
    SetTargetFPS(60);

    Animator animator;

    DrawPileUIConfig drawConfig;
    drawConfig.transform.position = {140, 580};

    LeftHandUIConfig handConfig;
    handConfig.position = {640, 580};

    DiscardPileUIConfig discardConfig;
    discardConfig.transform.position = {1140, 580};

    DrawPileUI    drawPile(drawConfig);
    LeftHandUI    hand(animator, handConfig);
    DiscardPileUI discardPile(animator, discardConfig);

    // Seed the deck.
    for (uint32_t id = 1; id <= 12; ++id)
        drawPile.add(CardUI(id));

    // Transient ghost cards for the reshuffle flourish; live only while animating.
    std::list<TransformComponent> reshuffleGhosts;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        animator.update(dt);

        // Draw the top card into the hand.
        if (IsKeyPressed(KEY_D) && !drawPile.empty() &&
            hand.cards().size() < (size_t)handConfig.slotCount)
            hand.addCard(drawPile.removeTopCard());

        // Discard the oldest card of the hand (the far end from slot 0).
        if (IsKeyPressed(KEY_E) && !hand.cards().empty())
            discardPile.add(hand.removeCard(hand.cards().back().id()));

        // Reshuffle the whole discard pile back into the deck.
        if (IsKeyPressed(KEY_R) && !discardPile.empty()) {
            Vector2 from = discardConfig.transform.position;
            Vector2 to   = drawConfig.transform.position;

            // Logical move is instant.
            std::vector<CardUI> cards;
            while (!discardPile.empty())
                cards.push_back(discardPile.removeTopCard());

            static std::mt19937 rng{std::random_device{}()};
            std::shuffle(cards.begin(), cards.end(), rng);

            for (auto& card : cards)
                drawPile.add(std::move(card));

            // One flourish: a few ghost cards sweep discard -> deck.
            for (int i = 0; i < 3; ++i) {
                auto it = reshuffleGhosts.insert(reshuffleGhosts.end(), TransformComponent{});
                it->position = from;

                Animation anim;
                anim.duration = 0.35f;
                anim.delay    = 0.08f * i;
                anim.easing   = Easing::easeInOut;
                anim.channels = {
                    {from.x, to.x, [it](float v) { it->position.x = v; }},
                    {from.y, to.y, [it](float v) { it->position.y = v; }},
                };
                anim.onComplete = [&reshuffleGhosts, it]() { reshuffleGhosts.erase(it); };
                animator.add(std::move(anim));
            }
        }

        BeginDrawing();
        ClearBackground({20, 20, 30, 255});

        drawSlotGhosts(hand);

        drawPileBack(drawConfig.transform.position,    drawPile.count(),    {60, 60, 80, 255}, "draw");
        drawPileBack(discardConfig.transform.position, discardPile.count(), {80, 30, 30, 255}, "discard");

        for (const auto& card : discardPile.cards())
            drawCard(card, {120, 60, 60, 255});

        for (const auto& card : hand.cards())
            drawCard(card, {70, 100, 140, 255});

        for (const auto& ghost : reshuffleGhosts) {
            Vector2 pos = ghost.position;
            DrawRectanglePro({pos.x, pos.y, 60, 90}, {30, 45}, ghost.rotation, {120, 120, 160, 180});
        }

        DrawText("D - draw a card    E - discard a card    R - reshuffle discard into deck", 10, 10, 16, LIGHTGRAY);
        DrawText(TextFormat("Deck: %d    Hand: %d    Discard: %d    Animations: %d",
                            drawPile.count(), (int)hand.cards().size(), discardPile.count(),
                            animator.activeCount()),
                 10, 35, 16, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
