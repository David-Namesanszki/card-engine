#include "scenes/battleScene.h"
#include "raylib.h"
#include "core/battle.h"
#include "core/captain.h"
#include "engine/systems/renderSystem.h"

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

BattleScene::BattleScene(
    Captain& captain,
    Battle& battle,
    SceneManager& scenes,
    std::optional<uint32_t> demoPlayerUnitId,
    std::optional<uint32_t> demoEnemyUnitId
)
    : _battle(battle),
      _scenes(scenes),
      _demoPlayerUnitId(demoPlayerUnitId),
      _demoEnemyUnitId(demoEnemyUnitId) {
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

    _ui = std::make_unique<BattleUI>(
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

    // The UI only reacts to core events. The events don't carry the texture or
    // pile sizes the UI methods need, so each lambda reads those from the core's
    // getters (and routes discards to the correct hand).
    // TODO: card texture is a placeholder until the event/core carries splash art.
    battle.onCardDrawn([this](CardDrawnEvent e) {
        _ui->drawCard(e.cardId, "assets/cards/FireBall.png", _battle.getDrawPile().size());
    });
    battle.onCardDiscarded([this](CardDiscardedEvent e) {
        int discardSize = _battle.getDiscardPile().size();
        if (e.hand == HandType::Left)
            _ui->discardFromLeftHand(e.cardId, discardSize);
        else
            _ui->discardFromRightHand(e.cardId, discardSize);
    });
    battle.onCardTransferredToRight([this](CardTransferredToRightEvent e) {
        _ui->transferCardToRight(e.cardId);
    });
    battle.onDrawPileRefilled([this](DrawPileRefilledEvent) {
        _ui->refillDrawPile(_battle.getDrawPile().size(), _battle.getDiscardPile().size());
    });
    battle.onBattleInfoChanged([this](BattleInfoChangedEvent e) {
        _ui->setDifficulty(difficultyLabel(e.info.difficulty));
        _ui->setWhoseTurn(turnTypeLabel(e.info.turnType));
        _ui->setBattleLength(e.info.turnCount);
    });
    battle.onActionPointsChanged([this](ActionPointsChangedEvent e) {
        _ui->setActionPoints(e.actionPoints.current, e.actionPoints.max);
    });
    battle.onFirePointsChanged([this](FirePointsChangedEvent e) {
        _ui->setFireCount(e.firePoints);
    });
    captain.onHealthChanged([this](HealthChangedEvent e) {
        _ui->setCaptainHealth(e.health.current, e.health.max);
    });

    // Unit events. UnitPlacedEvent carries a snapshot of the unit, so the UI
    // reads stats straight from the payload; positions come from the board's
    // hex-to-screen mapping.
    // TODO: every unit is a viking until per-unit-type art (and a unit kind
    // in core to select it) exists.
    battle.onUnitPlaced([this](UnitPlacedEvent e) {
        const char* texture = "assets/units/viking.png";
        _ui->placeUnit(
            e.unitId,
            e.unit.getName(),
            texture,
            _ui->board().unitPosition(e.at),
            e.unit.getHealth().current,
            e.unit.getAttackPower(),
            e.unit.getArmor().getDefensivePower()
        );
    });
    battle.onUnitMoved([this](UnitMovedEvent e) {
        _ui->moveUnit(e.unitId, _ui->board().unitPosition(e.to));
    });
    battle.onUnitDamaged([this](UnitDamagedEvent e) {
        _ui->setUnitHealth(e.unitId, e.health.current);
        _ui->setUnitArmor(e.unitId, e.currentArmor);
    });
    battle.onUnitHealed([this](UnitHealedEvent e) {
        _ui->setUnitHealth(e.unitId, e.health.current);
    });
    battle.onUnitDefended([this](UnitDefendedEvent e) {
        _ui->setUnitArmor(e.unitId, e.currentArmor);
    });
    battle.onUnitDied([this](UnitDiedEvent e) { _ui->removeUnit(e.unitId); });
}

void BattleScene::update(float) {
    if (IsKeyPressed(KEY_D)) {
        _battle.endPlayerTurn();
    }

    if (IsKeyPressed(KEY_T)) {
        _battle.startPlayerTurn();
    }

    // Combat demo keys, driving the first lane's units.
    if (IsKeyPressed(KEY_A) && _demoPlayerUnitId)
        _battle.attackWithUnit(*_demoPlayerUnitId);
    if (IsKeyPressed(KEY_G) && _demoEnemyUnitId)
        _battle.defendWithUnit(*_demoEnemyUnitId);
    if (IsKeyPressed(KEY_H) && _demoEnemyUnitId)
        _battle.healUnit(*_demoEnemyUnitId, 2);

    for (const InputEvent& event : _inputSystem.getInputs()) {
        std::optional<HitResult> hit = _hitTestSystem.hitTest(event.position, *_ui);
        if (!hit)
            continue;
        IntentType intentType = _intentSystem.interpret(event.type, hit->type);
        if (intentType == IntentType::TryPlayCard) {
            _playedCardId = hit->cardId;
            auto result = _battle.tryPlayCard(*_playedCardId);
            if (!result) {
                TraceLog(LOG_INFO, "Can't play card: %d", result.error());
            } else if (result.value().neededTargets == 0) {
                _battle.playCard(*_playedCardId, {});
            } else {
                _targetSystem.init(result.value().possibleTargets, result.value().neededTargets);
                _intentSystem.changeState(InputState::Target);
            }
        } else if (intentType == IntentType::SelectTarget) {
            _targetSystem.tryAddTarget(hit->cardId);
            if (_targetSystem.isFilled()) {
                _battle.playCard(*_playedCardId, _targetSystem.getSelectedTargets());
                _targetSystem.reset();
                _intentSystem.changeState(InputState::Normal);
            }
        } else if (intentType == IntentType::CancelCardPlay) {
            _playedCardId.reset();
            _targetSystem.reset();
            _intentSystem.changeState(InputState::Normal);
        } else if (intentType == IntentType::EndTurn) {
            _battle.endPlayerTurn();
        }
    }
}

void BattleScene::render(RenderSystem& renderer) {
    renderer.renderBattle(*_ui);
    if (_intentSystem.getInputState() == InputState::Target)
        renderer.renderTargetOverlay(*_ui, _targetSystem.getPossibleTargets());

    DrawText("T start turn   D end turn   A attack   G defend   H heal", 10, 10, 16, LIGHTGRAY);
}
