#include "raylib.h"
#include "UI/battleUI.h"
#include "UI/renderer.h"
#include <cstdint>

int main() {
    InitWindow(1280, 720, "Card Battle");
    SetTargetFPS(60);

    // ---- Configs ----
    BattleUIConfig battleConfig;

    HandUIConfig leftHandConfig;
    leftHandConfig.transform.position = {440.0f, 600.0f};
    leftHandConfig.slotCount = 5;
    leftHandConfig.sprite.texture = "assets/card.png";

    HandUIConfig rightHandConfig;
    rightHandConfig.transform.position = {840.0f, 600.0f};
    rightHandConfig.slotCount = 5;
    rightHandConfig.sprite.texture = "assets/card.png";

    CardPileUIConfig drawPileConfig;
    drawPileConfig.transform.position = {140.0f, 600.0f};
    drawPileConfig.sprite.texture = "assets/pile.png";

    CardPileUIConfig discardPileConfig;
    discardPileConfig.transform.position = {1140.0f, 600.0f};
    discardPileConfig.sprite.texture = "assets/pile.png";

    // Constructor order: battle, left hand, right hand, discard pile, draw pile.
    BattleUI battle(
        battleConfig, leftHandConfig, rightHandConfig, discardPileConfig, drawPileConfig
    );
    Renderer renderer;

    uint32_t nextCardId = 1;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // ---- Input ----
        if (IsKeyPressed(KEY_D))
            battle.drawCard(nextCardId++);

        if (IsKeyPressed(KEY_E)) {
            const std::list<uint32_t>& ids = battle.leftHand().getCardIds();
            if (!ids.empty())
                battle.discardFromLeftHand(ids.back());
        }

        // ---- Render ----
        BeginDrawing();
        ClearBackground({20, 20, 30, 255});

        renderer.render(battle);

        DrawText("D - draw a card    E - discard from left hand", 10, 10, 16, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
