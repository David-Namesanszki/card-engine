#include "raylib.h"
#include "renderer.h"
#include "battle.h"
#include "raid/captain.h"

int main() {
    InitWindow(1280, 720, "Card Engine");
    SetTargetFPS(60);

    CardPile deck;
    for (uint32_t i = 0; i < 14; i++)
        deck.addCard(i);

    Captain captain;
    Battle battle(captain, 0, deck);
    Renderer renderer;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE))
            battle.startPlayerTurn();

        if (IsKeyPressed(KEY_E))
            battle.endPlayerTurn();

        BeginDrawing();
        renderer.drawBattle(battle);
        DrawText("SPACE - Start Turn    E - End Turn", 10, 10, 16, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
