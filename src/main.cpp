#include "raylib.h"
#include "UI/battleUI.h"
#include "engine/systems/renderSystem.h"
#include "engine/systems/inputSystem.h"
#include "engine/systems/hitTestSystem.h"
#include <cstdint>
#include <map>
#include <optional>
#include "engine/systems/intentSystem.h"
#include "engine/systems/targetSystem.h"
#include "core/battle.h"

int main() {
    InitWindow(1280, 720, "Card Battle");
    SetTargetFPS(60);

    // ---- Configs ----
    BattleUIConfig battleConfig;

    // Fire resource panel: top-left corner.
    FireResourcePanelUIConfig firePanelConfig;
    firePanelConfig.transform.position = {120.0f, 40.0f};
    firePanelConfig.background.texture = "assets/FirePanel.png";
    firePanelConfig.background.size = {90.0f, 60.0f};

    // Action points panel: straight line next to the fire panel.
    ActionPointsPanelUIConfig actionPointsConfig;
    actionPointsConfig.transform.position = {320.0f, 40.0f};
    actionPointsConfig.background.texture = "assets/action_point_panel.png";
    actionPointsConfig.background.size = {140.0f, 60.0f};
    actionPointsConfig.availableTexture = "assets/action_point.png";
    actionPointsConfig.spentTexture = "assets/action_point_spent.png";
    actionPointsConfig.maxActionPoints = 3;
    actionPointsConfig.lineStartOffset = {-40.0f, 0.0f};
    actionPointsConfig.lineEndOffset = {40.0f, 0.0f};
    actionPointsConfig.pipSize = {24.0f, 24.0f};

    // Progress panel: top-centre, tracking day/raid/resource counts.
    ProgressPanelUIConfig progressPanelConfig;
    progressPanelConfig.transform.position = {640.0f, 40.0f};
    progressPanelConfig.background.texture = "assets/ProgressPanel.png";
    progressPanelConfig.background.size = {300.0f, 60.0f};

    // Battle info panel: top-right corner, tracking difficulty/turn/length.
    BattleInfoPanelUIConfig battleInfoPanelConfig;
    battleInfoPanelConfig.transform.position = {1100.0f, 40.0f};
    battleInfoPanelConfig.background.texture = "assets/BattleInfoPanel.png";
    battleInfoPanelConfig.background.size = {200.0f, 60.0f};

    // End turn button: right-centre, between the right hand and discard pile.
    ButtonUIConfig endTurnButtonConfig;
    endTurnButtonConfig.transform.position = {1160.0f, 360.0f};
    endTurnButtonConfig.sprite.texture = "assets/EndTurnButton.png";
    endTurnButtonConfig.sprite.size = {120.0f, 60.0f};

    // Window is 1280x720; centre x = 640.
    // Cards sit on an arc spanning handPos.x +/- 200, with the middle slot
    // pulled toward controlOffset.y. Keep hands clear of the top/bottom edges
    // (a card is ~110 tall and drawn centred on its slot).

    // Left hand: player's hand, bottom-centre.
    HandUIConfig leftHandConfig;
    leftHandConfig.transform.position = {640.0f, 600.0f};
    leftHandConfig.slotCount = 5;
    leftHandConfig.sprite.texture = "assets/LeftHandCover.png";
    leftHandConfig.sprite.size = {200.0f, 52.0f}; // cover art is wide (~520x134)

    // Right hand: opponent's hand, top-centre. Arc bows downward (toward the
    // middle of the screen) so the cards stay below the top edge.
    HandUIConfig rightHandConfig;
    rightHandConfig.transform.position = {640.0f, 120.0f};
    rightHandConfig.controlOffset = {0.0f, 60.0f};
    rightHandConfig.slotCount = 5;
    rightHandConfig.sprite.texture = "assets/RightHandCover.png";
    rightHandConfig.sprite.size = {200.0f, 52.0f};

    // Draw pile: bottom-left corner.
    CardPileUIConfig drawPileConfig;
    drawPileConfig.transform.position = {120.0f, 600.0f};
    drawPileConfig.sprite.texture = "assets/DrawPile.png";
    drawPileConfig.sprite.size = {90.0f, 113.0f};

    // Discard pile: bottom-right corner.
    CardPileUIConfig discardPileConfig;
    discardPileConfig.transform.position = {1160.0f, 600.0f};
    discardPileConfig.sprite.texture = "assets/DiscardPile.png";
    discardPileConfig.sprite.size = {90.0f, 113.0f};

    // Core battle: a Captain, fire points, and a deck of card ids.
    Captain captain;
    CardPile deck;
    for (uint32_t id = 0; id < 10; ++id)
        deck.addCard(id);
    const int startingFirePoints = 3;
    Battle battle(captain, startingFirePoints, deck);

    // Board: centre of the screen, between the two hands.
    BoardUIConfig boardConfig;
    boardConfig.transform.position = {640.0f, 360.0f};

    BattleUI battleUI(
        battleConfig,
        boardConfig,
        firePanelConfig,
        actionPointsConfig,
        progressPanelConfig,
        battleInfoPanelConfig,
        endTurnButtonConfig,
        leftHandConfig,
        rightHandConfig,
        discardPileConfig,
        drawPileConfig
    );
    // NOTE: Battle doesn't expose fire points or spent action points (no
    // getter/event for either), so neither panel can track changes yet —
    // fire is seeded from the same value passed to Battle; action points
    // start fully available.
    battleUI.setFireCount(startingFirePoints);
    battleUI.setActionPointsSpent(0);

    // BattleUI only reacts to core events. The events don't carry the texture or
    // pile sizes the UI methods need, so each lambda reads those from the core's
    // getters (and routes discards to the correct hand).
    // TODO: card texture is a placeholder until the event/core carries splash art.
    battle.onCardDrawn([&battleUI, &battle](CardDrawnEvent e) {
        battleUI.drawCard(e.cardId, "assets/cards/FireBall.png", battle.getDrawPile().size());
    });
    battle.onCardDiscarded([&battleUI, &battle](CardDiscardedEvent e) {
        int discardSize = battle.getDiscardPile().size();
        if (e.hand == HandType::Left)
            battleUI.discardFromLeftHand(e.cardId, discardSize);
        else
            battleUI.discardFromRightHand(e.cardId, discardSize);
    });
    battle.onCardTransferredToRight([&battleUI](CardTransferredToRightEvent e) {
        battleUI.transferCardToRight(e.cardId);
    });
    battle.onDrawPileRefilled([&battleUI, &battle](DrawPileRefilledEvent) {
        battleUI.refillDrawPile(battle.getDrawPile().size(), battle.getDiscardPile().size());
    });

    RenderSystem renderer;
    InputSystem inputSystem;
    HitTestSystem hitTestSystem;
    TargetSystem targetSystem;
    IntentSystem intentSystem;
    std::optional<uint32_t> playedCardId;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (IsKeyPressed(KEY_D)) {
            battle.endPlayerTurn();
        }

        if (IsKeyPressed(KEY_T)) {
            battle.startPlayerTurn();
        }

        for (const InputEvent& event : inputSystem.getInputs()) {
            std::optional<HitResult> hit = hitTestSystem.hitTest(event.position, battleUI);
            if (!hit)
                continue;
            IntentType intentType = intentSystem.interpret(event.type, hit->type);
            if (intentType == IntentType::TryPlayCard) {
                playedCardId = hit->cardId;
                auto result = battle.tryPlayCard(*playedCardId);
                if (!result) {
                    TraceLog(LOG_INFO, "Can't play card: %d", result.error());
                } else if (result.value().neededTargets == 0) {
                    battle.playCard(*playedCardId, {});
                } else {
                    targetSystem.init(result.value().possibleTargets, result.value().neededTargets);
                    intentSystem.changeState(InputState::Target);
                }
            } else if (intentType == IntentType::SelectTarget) {
                targetSystem.tryAddTarget(hit->cardId);
                if (targetSystem.isFilled()) {
                    battle.playCard(*playedCardId, targetSystem.getSelectedTargets());
                    targetSystem.reset();
                    intentSystem.changeState(InputState::Normal);
                }
            } else if (intentType == IntentType::CancelCardPlay) {
                playedCardId.reset();
                targetSystem.reset();
                intentSystem.changeState(InputState::Normal);
            } else if (intentType == IntentType::EndTurn) {
                battle.endPlayerTurn();
            }
        }

        // ---- Render ----
        BeginDrawing();
        ClearBackground({20, 20, 30, 255});

        renderer.renderBattle(battleUI);
        if (intentSystem.getInputState() == InputState::Target)
            renderer.renderTargetOverlay(battleUI, targetSystem.getPossibleTargets());

        DrawText(
            "D draw   T transfer L>R   E discard L   F discard R   R reshuffle",
            10,
            10,
            16,
            LIGHTGRAY
        );

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
