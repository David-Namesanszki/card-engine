#include "scenes/battleScene.h"
#include "raylib.h"
#include "core/captain.h"
#include "core/battle.h"
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

BattleScene::BattleScene(BattleLayout layout)
    : _drawPile(layout.drawPileConfig),
      _discardPile(layout.discardPileConfig),
      _leftHand(layout.leftHandConfig),
      _rightHand(layout.rightHandConfig),
      _board(layout.boardConfig),
      _firePointsPanel(layout.firePanelConfig),
      _actionPointsPanel(layout.actionPointsConfig),
      _progressPanel(layout.progressPanelConfig),
      _battleInfoPanel(layout.battleInfoPanelConfig),
      _endTurnButton(layout.endTurnButtonConfig),
      _captain(layout.captainConfig),
      _layout(layout) {
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
}

void BattleScene::start(const Captain& captain, const Battle& battle) {
    _cardFlowSystem = {captain.getDeck(), captain.getLeftHandSize(), captain.getRightHandSize()};
    _cardFlowSystem.onCardDrawn([this](CardDrawnEvent e) {
        CardUI c(e.card.getId());
        c.transform = _drawPile.getAnchorPoint().transform.asWorldTransform();
        c.sprite.texture = _layout.cardConfig.texture;
        c.hitbox.setRectangle(c.sprite.size);

        _cards.push_back(c);
        _drawPile.setCount(e.drawPileSize);
        _leftHand.addCardId(e.cardId, 0);
        // reorganizeHand(_leftHand);
    });
    _cardFlowSystem.onCardDiscarded([this](CardDiscardedEvent e) {
        int discardSize = _battle.getDiscardPile().size();
        if (e.hand == HandType::Left) {
            _leftHand.removeCardId(e.cardId);
            // takeCard(cardId);
            _discardPile.setCount(e.discardPileSize);
            // reorganizeHand(_leftHand);
        } else {
            _rightHand.removeCardId(e.cardId);
            // takeCard(cardId);
            _discardPile.setCount(e.discardPileSize);
            // reorganizeHand(_leftHand);
        }
    });
    _cardFlowSystem.onCardTransferredToRight([this](CardTransferredToRightEvent e) {
        _leftHand.removeCardId(e.cardId);
        // reorganizeHand(_leftHand);
        _rightHand.addCardId(e.cardId, 0);
        // reorganizeHand(_rightHand);
    });
    _cardFlowSystem.onDrawPileRefilled([this](DrawPileRefilledEvent e) {
        _drawPile.setCount(e.drawPileSize);
        _discardPile.setCount(e.discardPileSize);
    });

    _turnSystem = {};
    _turnSystem.onTurnCountIncremented([this](TurnCountIncrementedEvent e) {
        _battleInfoPanel.setBattleLength(e.turnCount);
    });
    _turnSystem.onWhoseTurnchanged([this](WhoseTurnChangedEvent e) {
        _battleInfoPanel.setWhoseTurn(e.whoseTurn);
    });

    _battleResourceSystem = {captain.getMaxActionPointCount(), captain.getFirePointCount()};
    _battleResourceSystem.onCurrentActionPointsChanged([this](CurrentActionPointsChangedEvent e) {
        _actionPointsPanel.setCurrentActionPoints(e.currentActionPoints);
    });
    _battleResourceSystem.onMaxActionPointsChanged([this](MaxActionPointsChangedEvent e) {
        _actionPointsPanel.setMaxActionPoints(e.maxActionPoints);
    });
    _battleResourceSystem.onCurrentFirePointsChanged([this](CurrentFirePointsChangedEvent e) {
        _firePointsPanel.setFireCount(e.currentFirePoints);
    });

    _effectSystem = {battle.boardPieces, battle.boardTiles};
    _effectSystem.onUnitDamaged([this](UnitDamagedEvent e) {
        auto it = std::find_if(_units.begin(), _units.end(), [e](const UnitUI& unit) {
            return unit.id() == e.id;
        });

        if (it == _units.end())
            return;

        it->setArmor(e.currentArmor);
        it->setHealth(e.currentHealth);
    });
    _effectSystem.onUnitHealed([this](UnitHealedEvent e) {
        auto it = std::find_if(_units.begin(), _units.end(), [e](const UnitUI& unit) {
            return unit.id() == e.id;
        });

        if (it == _units.end())
            return;

        it->setHealth(e.currentHealth);
    });
    _effectSystem.onUnitDefended([this](UnitDefendedEvent e) {
        auto it = std::find_if(_units.begin(), _units.end(), [e](const UnitUI& unit) {
            return unit.id() == e.id;
        });

        if (it == _units.end())
            return;

        it->setArmor(e.currentArmor);
    });
    _effectSystem.onUnitDied([this](UnitDiedEvent e) {
        auto it = std::find_if(_units.begin(), _units.end(), [e](const UnitUI& unit) {
            return unit.id() == e.id;
        });

        if (it == _units.end())
            return;

        _units.erase(it);
    });
}

void BattleScene::update(float) {
    if (IsKeyPressed(KEY_D)) {
        _cardFlowSystem.drawCard(5);
        _battleResourceSystem.refillActionPoints();
        _turnSystem.incrementTurnCount();
        _turnSystem.changeWhoseTurn();
    }

    if (IsKeyPressed(KEY_T)) {
        _cardFlowSystem.discardLeftHand();
        _cardFlowSystem.discardRightHand();
        _turnSystem.changeWhoseTurn();
    }

    // for (const InputEvent& event : _inputSystem.getInputs()) {
    //     std::optional<HitResult> hit = _hitTestSystem.hitTest(event.position, *_ui);
    //     if (!hit)
    //         continue;
    //     IntentType intentType = _intentSystem.interpret(event.type, hit->type);
    //     if (intentType == IntentType::TryPlayCard) {
    //         _playedCardId = hit->cardId;
    //         auto result = _battle.tryPlayCard(*_playedCardId);
    //         if (!result) {
    //             TraceLog(LOG_INFO, "Can't play card: %d", result.error());
    //         } else if (result.value().neededTargets == 0) {
    //             _battle.playCard(*_playedCardId, {});
    //         } else {
    //             _targetSystem.init(result.value().possibleTargets, result.value().neededTargets);
    //             _intentSystem.changeState(InputState::Target);
    //         }
    //     } else if (intentType == IntentType::SelectTarget) {
    //         _targetSystem.tryAddTarget(hit->cardId);
    //         if (_targetSystem.isFilled()) {
    //             _battle.playCard(*_playedCardId, _targetSystem.getSelectedTargets());
    //             _targetSystem.reset();
    //             _intentSystem.changeState(InputState::Normal);
    //         }
    //     } else if (intentType == IntentType::CancelCardPlay) {
    //         _playedCardId.reset();
    //         _targetSystem.reset();
    //         _intentSystem.changeState(InputState::Normal);
    //     } else if (intentType == IntentType::EndTurn) {
    //         _battle.endPlayerTurn();
    //     }
    // }
}

void BattleScene::render(RenderSystem& renderer) {
    renderBoard(_board);

    renderCaptain(_captain);
    for (const auto& unit : _units)
        renderUnit(unit);

    renderCardPile(_drawPile);
    renderCardPile(_discardPile);
    renderHand(_leftHand);
    renderHand(_rightHand);

    for (const auto& card : _cards)
        renderCard(card);

    renderResourcePanel(_firePointsPanel);
    renderActionPointsPanel(_actionPointsPanel);
    renderProgressPanel(_progressPanel);
    renderBattleInfoPanel(_battleInfoPanel);

    renderButton(_endTurnButton);
    if (_intentSystem.getInputState() == InputState::Target) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 150});

        for (const auto& card : _cards) {
            bool targetable =
                std::find(targetableCardIds.begin(), targetableCardIds.end(), card.id()) !=
                targetableCardIds.end();
            if (targetable)
                draw(card.transform, card.sprite);
        }
    }

    DrawText("T start turn   D end turn   A attack   G defend   H heal", 10, 10, 16, LIGHTGRAY);
}
