#include "raylib.h"
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>
#include "engine/sceneManager.h"
#include "engine/systems/renderSystem.h"
#include "scenes/tacticalScene.h"
#include "core/captain.h"
#include "scenes/battleScene.h"

BattleLayout defaultBattleLayout() {
    BattleLayout layout;

    // Battle info panel: left column, top (0.1 x 0.3).
    layout.battleInfoPanelConfig.transform.position = {64.0f, 108.0f};
    layout.battleInfoPanelConfig.background.texture = "assets/BattleInfoPanel.png";
    layout.battleInfoPanelConfig.background.size = {128.0f, 216.0f};

    // TODO: BattleLog (0.1 x 0.3) sits below BattleInfo in the layout, but no
    // widget exists for it yet.

    // Draw pile: left column, bottom (0.1 x 0.2).
    layout.drawPileConfig.transform.position = {64.0f, 648.0f};
    layout.drawPileConfig.sprite.texture = "assets/DrawPile.png";
    layout.drawPileConfig.sprite.size = {128.0f, 144.0f};

    // Board: centre column, upper region (0.8 x 0.7).
    layout.boardConfig.transform.position = {640.0f, 252.0f};

    // Bottom row of the centre column (0.3 tall): left hand, captain, right hand.
    // Cards sit on an arc spanning handPos.x +/- 200, with the middle slot
    // pulled toward controlOffset.y.
    layout.leftHandConfig.transform.position = {320.0f, 612.0f}; // (0.3 x 0.3)
    layout.leftHandConfig.slotCount = 5;
    layout.leftHandConfig.sprite.texture = "assets/LeftHandCover.png";
    layout.leftHandConfig.sprite.size = {384.0f, 99.0f}; // region width, cover aspect (~520x134)

    layout.captainConfig.transform.position = {640.0f, 612.0f}; // (0.2 x 0.3)
    layout.captainConfig.sprite.texture = "assets/Captain.png";
    layout.captainConfig.sprite.size = {256.0f, 216.0f};

    layout.rightHandConfig.transform.position = {960.0f, 612.0f}; // (0.3 x 0.3)
    layout.rightHandConfig.slotCount = 5;
    layout.rightHandConfig.sprite.texture = "assets/RightHandCover.png";
    layout.rightHandConfig.sprite.size = {384.0f, 99.0f};

    // Progress panel: right column, top (0.1 x 0.3).
    layout.progressPanelConfig.transform.position = {1216.0f, 108.0f};
    layout.progressPanelConfig.background.texture = "assets/ProgressPanel.png";
    layout.progressPanelConfig.background.size = {128.0f, 216.0f};

    // End turn button: right column, second slot (0.1 x 0.1).
    layout.endTurnButtonConfig.transform.position = {1216.0f, 300.0f};
    layout.endTurnButtonConfig.sprite.texture = "assets/EndTurnButton.png";
    layout.endTurnButtonConfig.sprite.size = {128.0f, 72.0f};

    // The layout's BattleResourcePanel slot (0.1 x 0.2) is two stacked panels:
    // fire resources on top, action points below, each 0.1 x 0.1.
    layout.firePanelConfig.transform.position = {1216.0f, 420.0f};
    layout.firePanelConfig.background.texture = "assets/FirePanel.png";
    layout.firePanelConfig.background.size = {128.0f, 72.0f};

    layout.actionPointsConfig.transform.position = {1216.0f, 492.0f};
    layout.actionPointsConfig.background.texture = "assets/action_point_panel.png";
    layout.actionPointsConfig.background.size = {128.0f, 72.0f};
    layout.actionPointsConfig.availableTexture = "assets/action_point.png";
    layout.actionPointsConfig.spentTexture = "assets/action_point_spent.png";
    layout.actionPointsConfig.lineStartOffset = {-40.0f, 0.0f};
    layout.actionPointsConfig.lineEndOffset = {40.0f, 0.0f};
    layout.actionPointsConfig.pipSize = {24.0f, 24.0f};

    // Discard pile: right column, bottom (0.1 x 0.2).
    layout.discardPileConfig.transform.position = {1216.0f, 648.0f};
    layout.discardPileConfig.sprite.texture = "assets/DiscardPile.png";
    layout.discardPileConfig.sprite.size = {128.0f, 144.0f};

    // Units on the board and cards in hand keep UnitUIConfig's / the sprite
    // component's own defaults; CardUIConfig has no default member values, so
    // it is set explicitly rather than left indeterminate.
    layout.cardConfig.width = 80.0f;
    layout.cardConfig.height = 110.0f;

    return layout;
}

int main() {
    const float screenW = 1280.0f;
    const float screenH = 720.0f;
    InitWindow((int)screenW, (int)screenH, "Card Battle");
    SetTargetFPS(60);

    SceneManager scenes;

    // Placeholder deck: 14 cards built from the two card arts that exist in
    // assets/cards. Names, descriptions and costs are stand-ins until real card
    // content exists — nothing reads them yet, since Card has no accessors.
    // Deliberately unshuffled so the draw order (and the event log) is
    // reproducible; call deck.shuffle() for a real one.
    CardPile deck;
    for (int i = 0; i < 7; ++i) {
        deck.addCard(Card(
            CardType::Command,
            "Archer Volley",
            "Deal 3 damage to a unit.",
            "assets/cards/CommandCardTemplate.png",
            1
        ));
        deck.addCard(Card(
            CardType::Fire,
            "Fireball",
            "Deal 5 damage to a unit.",
            "assets/cards/DreamCardTemplate.png",
            2
        ));
    }

    std::vector<std::unique_ptr<BoardPiece>> boardPieces;

    boardPieces.emplace_back(
        std::make_unique<Unit>("Archers", TeamType::Player, Health{8, 8}, HexCoord{-1, 1}, 4, 2)
    );
    boardPieces.emplace_back(
        std::make_unique<Unit>("Raiders", TeamType::Player, Health{8, 9}, HexCoord{-1, 0}, 5, 1)
    );
    boardPieces.emplace_back(
        std::make_unique<Unit>("Viking", TeamType::Player, Health{3, 8}, HexCoord{1, 1}, 3, 1)
    );

    std::vector<BoardTile> boardTiles = {
        {{-1, 1}, TeamType::Player, BoardTileType::Unit},
        {{-1, 0}, TeamType::Enemy, BoardTileType::Unit},
        {{0, 0}, TeamType::Neutral, BoardTileType::Effect},
        {{1, 0}, TeamType::Player, BoardTileType::Unit},
        {{1, -1}, TeamType::Enemy, BoardTileType::Unit},
    };
    scenes.requestChange(
        std::make_unique<BattleScene>(
            scenes,
            BattleData{
                deck,
                5,
                5,
                TurnType::Player,
                BattleDifficultyType::Minor,
                Health{20, 30},
                4,
                20,
                std::move(boardPieces),
                boardTiles
            },
            defaultBattleLayout()
        )
    );
    RenderSystem renderer;
    // The run opens on the tactical (deployment) view; it hands the battle
    // over to BattleScene itself.
    scenes.applyPending();
    // The scene wired itself in its constructor; announce the opening state.

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
