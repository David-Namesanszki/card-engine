#include "raylib.h"
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>
#include "core/battle.h"
#include "engine/sceneManager.h"
#include "engine/systems/renderSystem.h"
#include "scenes/tacticalScene.h"

int main() {
    const float screenW = 1280.0f;
    const float screenH = 720.0f;
    InitWindow((int)screenW, (int)screenH, "Card Battle");
    SetTargetFPS(60);

    // ---- Run-scoped state: outlives any single scene ----
    // The Captain owns the persistent deck; each battle copies it into its
    // own draw pile.
    std::vector<uint32_t> starterDeck;
    for (uint32_t id = 0; id < 14; ++id)
        starterDeck.push_back(id);

    // Starter roster: what the player can deploy during the tactical phase.
    Roster roster(10);
    roster.addUnit(Unit("Archers", TeamType::Player, Health{8, 8}, 4, 2));
    roster.addUnit(Unit("Vikings", TeamType::Player, Health{10, 10}, 3, 3));
    roster.addUnit(Unit("Berserkers", TeamType::Player, Health{7, 7}, 5, 1));
    roster.addUnit(Unit("Shieldmen", TeamType::Player, Health{12, 12}, 2, 4));
    roster.addConstruction(Construction("Palisade", TeamType::Player, Durability(10)));
    roster.addConstruction(Construction("Watchtower", TeamType::Player, Durability(8)));

    Captain captain("Captain", Health{30, 30}, starterDeck, roster);
    CardPile deck;
    for (uint32_t id : captain.getDeck())
        deck.addCard(id);

    // Board layout: content, defined once and fed to the core; the UI reads
    // it back through battle.getBoard().
    std::vector<BoardTile> boardLayout = {
        {{-1, 1}, BoardTileType::Unit, TeamType::Player},
        {{-1, 0}, BoardTileType::Unit, TeamType::Enemy},
        {{0, 0}, BoardTileType::Effect, TeamType::Neutral},
        {{1, 0}, BoardTileType::Unit, TeamType::Player},
        {{1, -1}, BoardTileType::Unit, TeamType::Enemy},
    };

    const int startingFirePoints = 3;
    const int maxActionPoints = 3;
    // Player units attack "up" the lane (toward decreasing r); enemies attack
    // back along the negation.
    Battle battle(
        captain,
        startingFirePoints,
        deck,
        BattleDifficultyType::Minor,
        maxActionPoints,
        Board(boardLayout, {0, -1})
    );

    // Demo units on the four unit tiles so the combat wiring is visible.
    // Placed before the UI exists — battle.startBattle() announces them.
    auto playerUnitId =
        battle.placeUnit(Unit("Archers", TeamType::Player, Health{8, 8}, 4, 2), {-1, 1});
    auto enemyUnitId =
        battle.placeUnit(Unit("Raiders", TeamType::Enemy, Health{6, 6}, 2, 1), {-1, 0});
    battle.placeUnit(Unit("Archers", TeamType::Player, Health{8, 8}, 4, 2), {1, 0});
    battle.placeUnit(Unit("Raiders", TeamType::Enemy, Health{6, 6}, 2, 1), {1, -1});

    RenderSystem renderer;
    SceneManager scenes;
    // The run opens on the tactical (deployment) view; it hands the battle
    // over to BattleScene itself.
    scenes.requestChange(std::make_unique<TacticalScene>(
        captain,
        battle,
        scenes,
        playerUnitId ? std::optional<uint32_t>(playerUnitId.value()) : std::nullopt,
        enemyUnitId ? std::optional<uint32_t>(enemyUnitId.value()) : std::nullopt
    ));
    scenes.applyPending();
    // The scene wired itself in its constructor; announce the opening state.
    battle.startTacticalPhase();
    battle.startBattle();

    while (!WindowShouldClose()) {
        scenes.applyPending(); // scene swaps land between frames
        scenes.update(GetFrameTime());

        BeginDrawing();
        ClearBackground({20, 20, 30, 255});
        scenes.render(renderer);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
