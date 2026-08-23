#include "scenes/battleScene.h"
#include "raylib.h"
#include "UI/labels.h"
#include "engine/systems/renderSystem.h"
#include <utility>

// Init-list order must mirror the declaration order in the header: _layout is
// initialized before the UI members, which then read their configs out of it.
BattleScene::BattleScene(
    SceneManager& scenes,
    BattleData data,
    BattleLayout layout,
    BattleConfig config
)
    : _scenes(scenes),
      _layout(std::move(layout)),

      _inputSystem(),
      _hitTestSystem(),
      _targetSystem(),
      _intentSystem(),

      _cardFlowSystem(std::move(data.deck), data.leftHandSize, data.rightHandSize),
      _turnSystem(data.turnType),
      _battleResourceSystem(data.maxActionPointCount, data.currentFirePointCount),
      _effectSystem(std::move(data.selectedBoardPieces), std::move(data.boardTiles)),

      _handLayoutSystem(
          _layout.leftHandConfig,
          _layout.rightHandConfig,
          _layout.drawPileConfig,
          _layout.discardPileConfig
      ),

      _battleResourceViewSystem(
          _layout.firePanelConfig,
          _layout.actionPointsConfig,
          data.maxActionPointCount,
          data.currentFirePointCount
      ),

      _turnViewSystem(_layout.battleInfoPanelConfig, data.difficulty, data.turnType),

      _board(_layout.boardConfig),
      _progressPanel(_layout.progressPanelConfig),
      _endTurnButton(_layout.endTurnButtonConfig),
      _captain(_layout.captainConfig) {
    _cardFlowSystem.onCardDrawn([this](CardDrawnEvent e) {
        _handLayoutSystem.drawCard(e.card.getId(), e.card.getSplashArt(), e.drawPileSize);
    });
    _cardFlowSystem.onCardDiscarded([this](CardDiscardedEvent e) {
        _handLayoutSystem.discardCard(e.hand, e.cardId, e.discardPileSize);
    });
    _cardFlowSystem.onCardTransferredToRight([this](CardTransferredToRightEvent e) {
        _handLayoutSystem.transferCard(e.cardId);
    });
    _cardFlowSystem.onDrawPileRefilled([this](DrawPileRefilledEvent e) {
        _handLayoutSystem.refillDrawPile(e.drawPileSize, e.discardPileSize);
    });

    _turnSystem.onTurnAdvanced([this](TurnAdvancedEvent e) {
        _turnViewSystem.advanceTurn(e.whoseTurn, e.turnCount);
    });

    _battleResourceSystem.onActionPointsRefilled([this](ActionPointsRefilledEvent e) {
        _battleResourceViewSystem.refillActionPoints(e.current);
    });

    _battleResourceSystem.onResourcesSpent([this](ResourcesSpentEvent e) {
        _battleResourceViewSystem.spendResources(
            e.actionCost, e.actionPointsRemaining, e.fireCost, e.firePointsRemaining
        );
    });

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

    subscribeLogging();
}

// One tracing listener per core event. EventBus is multicast, so these ride
// alongside the widget subscriptions above without disturbing them, and the
// whole block can be dropped by deleting this call.
void BattleScene::subscribeLogging() {
    _cardFlowSystem.onCardDrawn([](CardDrawnEvent e) {
        TraceLog(LOG_INFO, "[card] drawn      id=%u drawPile=%zu", e.card.getId(), e.drawPileSize);
    });
    _cardFlowSystem.onCardDiscarded([](CardDiscardedEvent e) {
        TraceLog(
            LOG_INFO,
            "[card] discarded  id=%u discardPile=%zu hand=%s",
            e.cardId,
            e.discardPileSize,
            handTypeLabel(e.hand)
        );
    });
    _cardFlowSystem.onCardTransferredToRight([](CardTransferredToRightEvent e) {
        TraceLog(LOG_INFO, "[card] ->right    id=%u", e.cardId);
    });
    _cardFlowSystem.onDrawPileRefilled([](DrawPileRefilledEvent e) {
        TraceLog(
            LOG_INFO,
            "[card] refilled   drawPile=%zu discardPile=%zu",
            e.drawPileSize,
            e.discardPileSize
        );
    });

    _turnSystem.onTurnAdvanced([](TurnAdvancedEvent e) {
        TraceLog(
            LOG_INFO,
            "[turn] advanced   whoseTurn=%s count=%zu",
            turnTypeLabel(e.whoseTurn),
            e.turnCount
        );
    });

    _battleResourceSystem.onActionPointsRefilled([](ActionPointsRefilledEvent e) {
        TraceLog(LOG_INFO, "[res]  refilled   actionPts=%d/%d", e.current, e.max);
    });
    _battleResourceSystem.onResourcesSpent([](ResourcesSpentEvent e) {
        TraceLog(
            LOG_INFO,
            "[res]  spent      ap-%d fire-%d  left ap=%d fire=%d",
            e.actionCost,
            e.fireCost,
            e.actionPointsRemaining,
            e.firePointsRemaining
        );
    });

    _effectSystem.onUnitDamaged([](UnitDamagedEvent e) {
        TraceLog(
            LOG_INFO,
            "[unit] damaged    id=%u hp=%d armor=%d",
            e.id,
            e.currentHealth,
            e.currentArmor
        );
    });
    _effectSystem.onUnitHealed([](UnitHealedEvent e) {
        TraceLog(LOG_INFO, "[unit] healed     id=%u hp=%d", e.id, e.currentHealth);
    });
    _effectSystem.onUnitDefended([](UnitDefendedEvent e) {
        TraceLog(LOG_INFO, "[unit] defended   id=%u armor=%d", e.id, e.currentArmor);
    });
    _effectSystem.onUnitDied([](UnitDiedEvent e) {
        TraceLog(LOG_INFO, "[unit] died       id=%u", e.id);
    });
}

void BattleScene::update(float) {
    if (IsKeyPressed(KEY_D)) {
        _turnSystem.advanceTurn();
        _cardFlowSystem.drawCard();
        _battleResourceSystem.refillActionPoints();
    }

    if (IsKeyPressed(KEY_T)) {
        _cardFlowSystem.discardLeftHand();
        _cardFlowSystem.discardRightHand();
        _turnSystem.advanceTurn();
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
    _battleResourceViewSystem.render(renderer);
    _turnViewSystem.render(renderer);
    _handLayoutSystem.render(renderer);
    renderer.renderBoard(_board);

    renderer.renderCaptain(_captain);
    for (const auto& unit : _units)
        renderer.renderUnit(unit);

    renderer.renderProgressPanel(_progressPanel);

    renderer.renderButton(_endTurnButton);
    if (_intentSystem.getInputState() == InputState::Target) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 150});

        // for (const auto& card : _cards) {
        //     bool targetable =
        //         std::find(targetableCardIds.begin(), targetableCardIds.end(), card.id()) !=
        //         targetableCardIds.end();
        //     if (targetable)
        //         draw(card.transform, card.sprite);
        // }
    }

    DrawText("T start turn   D end turn   A attack   G defend   H heal", 10, 10, 16, LIGHTGRAY);
}

// ---- Layout ----
// The base battle layout. Each element owns a (width, height) fraction of the
// 1280x720 screen and is centred in its region; sprites stretch to fill it.
//
//   left column  (0.1 wide): BattleInfo 0.3 | BattleLog 0.3 | gap | DrawPile 0.2
//   centre       (0.8 wide): Board 0.7, then LeftHand 0.3 / Captain 0.2 / RightHand 0.3
//   right column (0.1 wide): ProgressPanel 0.3 | EndTurn 0.1 | FirePanel 0.1 +
//                            ActionPoints 0.1 | DiscardPile 0.2, with three even
//                            48px gaps between the four slots
