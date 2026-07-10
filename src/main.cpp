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

// Core speaks in enums; the info panel displays strings. The mapping lives on
// the UI side of the boundary.
static const char* turnTypeLabel(TurnType turnType) {
    return turnType == TurnType::Player ? "Player" : "Enemy";
}

static const char* difficultyLabel(BattleDifficultyType difficulty) {
    switch (difficulty) {
    case BattleDifficultyType::Minor:
        return "Minor";
    case BattleDifficultyType::Major:
        return "Major";
    case BattleDifficultyType::Boss:
        return "Boss";
    }
    return "Unknown";
}

int main() {
    const float screenW = 1280.0f;
    const float screenH = 720.0f;
    InitWindow((int)screenW, (int)screenH, "Card Battle");
    SetTargetFPS(60);

    // ---- Configs ----
    // Base battle layout. Each element owns a (width, height) fraction of the
    // 1280x720 screen and is centred in its region; sprites stretch to fill it.
    //
    //   left column  (0.1 wide): BattleInfo 0.3 | BattleLog 0.3 | gap | DrawPile 0.2
    //   centre       (0.8 wide): Board 0.7, then LeftHand 0.3 / Captain 0.2 / RightHand 0.3
    //   right column (0.1 wide): ProgressPanel 0.3 | EndTurn 0.1 | FirePanel 0.1 +
    //                            ActionPoints 0.1 | DiscardPile 0.2, with three even
    //                            48px gaps between the four slots
    BattleUIConfig battleConfig;

    // Battle info panel: left column, top (0.1 x 0.3).
    BattleInfoPanelUIConfig battleInfoPanelConfig;
    battleInfoPanelConfig.transform.position = {64.0f, 108.0f};
    battleInfoPanelConfig.background.texture = "assets/BattleInfoPanel.png";
    battleInfoPanelConfig.background.size = {128.0f, 216.0f};

    // TODO: BattleLog (0.1 x 0.3) sits below BattleInfo in the layout, but no
    // widget exists for it yet.

    // Draw pile: left column, bottom (0.1 x 0.2).
    CardPileUIConfig drawPileConfig;
    drawPileConfig.transform.position = {64.0f, 648.0f};
    drawPileConfig.sprite.texture = "assets/DrawPile.png";
    drawPileConfig.sprite.size = {128.0f, 144.0f};

    // Board layout: defined once, feeds both the core Board (rules) and the
    // BoardUI (rendering).
    std::vector<BoardTile> boardLayout = {
        {{-1, 1}, BoardTileType::Unit, TeamType::Player},
        {{-1, 0}, BoardTileType::Unit, TeamType::Enemy},
        {{0, 0}, BoardTileType::Effect, TeamType::Neutral},
        {{1, 0}, BoardTileType::Unit, TeamType::Player},
        {{1, -1}, BoardTileType::Unit, TeamType::Enemy},
    };

    // Board: centre column, upper region (0.8 x 0.7).
    BoardUIConfig boardConfig;
    boardConfig.transform.position = {640.0f, 252.0f};

    // Bottom row of the centre column (0.3 tall): left hand, captain, right hand.
    // Cards sit on an arc spanning handPos.x +/- 200, with the middle slot
    // pulled toward controlOffset.y.
    HandUIConfig leftHandConfig;
    leftHandConfig.transform.position = {320.0f, 612.0f}; // (0.3 x 0.3)
    leftHandConfig.slotCount = 5;
    leftHandConfig.sprite.texture = "assets/LeftHandCover.png";
    leftHandConfig.sprite.size = {384.0f, 99.0f}; // region width, cover aspect (~520x134)

    CaptainUIConfig captainConfig;
    captainConfig.transform.position = {640.0f, 612.0f}; // (0.2 x 0.3)
    captainConfig.sprite.texture = "assets/Captain.png";
    captainConfig.sprite.size = {256.0f, 216.0f};

    HandUIConfig rightHandConfig;
    rightHandConfig.transform.position = {960.0f, 612.0f}; // (0.3 x 0.3)
    rightHandConfig.slotCount = 5;
    rightHandConfig.sprite.texture = "assets/RightHandCover.png";
    rightHandConfig.sprite.size = {384.0f, 99.0f};

    // Progress panel: right column, top (0.1 x 0.3).
    ProgressPanelUIConfig progressPanelConfig;
    progressPanelConfig.transform.position = {1216.0f, 108.0f};
    progressPanelConfig.background.texture = "assets/ProgressPanel.png";
    progressPanelConfig.background.size = {128.0f, 216.0f};

    // End turn button: right column, second slot (0.1 x 0.1).
    ButtonUIConfig endTurnButtonConfig;
    endTurnButtonConfig.transform.position = {1216.0f, 300.0f};
    endTurnButtonConfig.sprite.texture = "assets/EndTurnButton.png";
    endTurnButtonConfig.sprite.size = {128.0f, 72.0f};

    // The layout's BattleResourcePanel slot (0.1 x 0.2) is two stacked panels:
    // fire resources on top, action points below, each 0.1 x 0.1.
    FireResourcePanelUIConfig firePanelConfig;
    firePanelConfig.transform.position = {1216.0f, 420.0f};
    firePanelConfig.background.texture = "assets/FirePanel.png";
    firePanelConfig.background.size = {128.0f, 72.0f};

    const int maxActionPoints = 3;

    ActionPointsPanelUIConfig actionPointsConfig;
    actionPointsConfig.transform.position = {1216.0f, 492.0f};
    actionPointsConfig.background.texture = "assets/action_point_panel.png";
    actionPointsConfig.background.size = {128.0f, 72.0f};
    actionPointsConfig.availableTexture = "assets/action_point.png";
    actionPointsConfig.spentTexture = "assets/action_point_spent.png";
    actionPointsConfig.lineStartOffset = {-40.0f, 0.0f};
    actionPointsConfig.lineEndOffset = {40.0f, 0.0f};
    actionPointsConfig.pipSize = {24.0f, 24.0f};

    // Discard pile: right column, bottom (0.1 x 0.2).
    CardPileUIConfig discardPileConfig;
    discardPileConfig.transform.position = {1216.0f, 648.0f};
    discardPileConfig.sprite.texture = "assets/DiscardPile.png";
    discardPileConfig.sprite.size = {128.0f, 144.0f};

    // Units on the board: shared readout layout; per-unit sprite textures are
    // chosen at placement.
    UnitUIConfig unitConfig;

    // Core battle: the Captain owns the persistent deck; each battle copies it
    // into its own draw pile.
    std::vector<uint32_t> starterDeck;
    for (uint32_t id = 0; id < 14; ++id)
        starterDeck.push_back(id);
    Captain captain("Captain", Health{30, 30}, starterDeck);
    CardPile deck;
    for (uint32_t id : captain.getDeck())
        deck.addCard(id);
    const int startingFirePoints = 3;
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

    BattleUI battleUI(
        battleConfig,
        boardConfig,
        battle.getBoard().tiles(),
        captainConfig,
        firePanelConfig,
        actionPointsConfig,
        progressPanelConfig,
        battleInfoPanelConfig,
        endTurnButtonConfig,
        leftHandConfig,
        rightHandConfig,
        discardPileConfig,
        drawPileConfig,
        unitConfig
    );

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
    battle.onBattleInfoChanged([&battleUI](BattleInfoChangedEvent e) {
        battleUI.setDifficulty(difficultyLabel(e.info.difficulty));
        battleUI.setWhoseTurn(turnTypeLabel(e.info.turnType));
        battleUI.setBattleLength(e.info.turnCount);
    });
    battle.onActionPointsChanged([&battleUI](ActionPointsChangedEvent e) {
        battleUI.setActionPoints(e.actionPoints.current, e.actionPoints.max);
    });
    battle.onFirePointsChanged([&battleUI](FirePointsChangedEvent e) {
        battleUI.setFireCount(e.firePoints);
    });
    captain.onHealthChanged([&battleUI](HealthChangedEvent e) {
        battleUI.setCaptainHealth(e.health.current, e.health.max);
    });

    // Unit events. UnitPlacedEvent carries a snapshot of the unit, so the UI
    // reads stats straight from the payload; positions come from the board's
    // hex-to-screen mapping.
    // TODO: every unit is an archer until per-unit-type art (and a unit kind
    // in core to select it) exists.
    battle.onUnitPlaced([&battleUI](UnitPlacedEvent e) {
        const char* texture = "assets/units/viking.png";
        battleUI.placeUnit(
            e.unitId,
            e.unit.getName(),
            texture,
            battleUI.board().unitPosition(e.at),
            e.unit.getHealth().current,
            e.unit.getAttackPower(),
            e.unit.getArmor().getDefensivePower()
        );
    });
    battle.onUnitMoved([&battleUI](UnitMovedEvent e) {
        battleUI.moveUnit(e.unitId, battleUI.board().unitPosition(e.to));
    });
    battle.onUnitDamaged([&battleUI](UnitDamagedEvent e) {
        battleUI.setUnitHealth(e.unitId, e.health.current);
        battleUI.setUnitArmor(e.unitId, e.currentArmor);
    });
    battle.onUnitHealed([&battleUI](UnitHealedEvent e) {
        battleUI.setUnitHealth(e.unitId, e.health.current);
    });
    battle.onUnitDefended([&battleUI](UnitDefendedEvent e) {
        battleUI.setUnitArmor(e.unitId, e.currentArmor);
    });
    battle.onUnitDied([&battleUI](UnitDiedEvent e) { battleUI.removeUnit(e.unitId); });

    // Everything is wired; the core announces its opening state through the
    // same buses the UI already listens to.
    battle.startBattle();

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

        // Combat demo keys, driving the first lane's units.
        if (IsKeyPressed(KEY_A) && playerUnitId)
            battle.attackWithUnit(playerUnitId.value());
        if (IsKeyPressed(KEY_G) && enemyUnitId)
            battle.defendWithUnit(enemyUnitId.value());
        if (IsKeyPressed(KEY_H) && enemyUnitId)
            battle.healUnit(enemyUnitId.value(), 2);

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

        DrawText("T start turn   D end turn   A attack   G defend   H heal", 10, 10, 16, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
