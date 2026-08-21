#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include "engine/scene.h"
#include "engine/systems/inputSystem.h"
#include "engine/systems/hitTestSystem.h"
#include "engine/systems/targetSystem.h"
#include "engine/systems/intentSystem.h"
#include "core/systems/cardFlowSystem.h"
#include "core/systems/battleResourceSystem.h"
#include "core/systems/turnSystem.h"

class Battle;
class Captain;
class SceneManager;

struct BattleLayout {
    BoardUIConfig boardConfig;
    FireResourcePanelUIConfig firePanelConfig;
    ActionPointsPanelUIConfig actionPointsConfig;
    ProgressPanelUIConfig progressPanelConfig;
    BattleInfoPanelUIConfig battleInfoPanelConfig;
    ButtonUIConfig endTurnButtonConfig;
    HandUIConfig leftHandConfig;
    HandUIConfig rightHandConfig;
    CardPileUIConfig discardPileConfig;
    CardPileUIConfig drawPileConfig;
    UnitUIConfig unitConfig;
    CaptainUIConfig captainConfig;
    CardUIConfig cardConfig;
};

class BattleScene : public Scene {
  public:
    BattleScene(BattleLayout layout);

    void start();
    void update(float dt) override;
    void render(RenderSystem& renderer) override;

  private:
    SceneManager& _scenes; // unused until scene transitions exist; shape set now

    BattleLayout _layout;

    // systems
    InputSystem _inputSystem;
    HitTestSystem _hitTestSystem;
    TargetSystem _targetSystem;
    IntentSystem _intentSystem;

    CardFlowSystem _cardFlowSystem;
    TurnSystem _turnSystem;
    BattleResourceSystem _battleResourceSystem;
    EffectSystem _effectSystem;
    // UI elements
    CardPileUI _drawPile;
    CardPileUI _discardPile;
    FixedSlotHandUI _leftHand;
    FixedSlotHandUI _rightHand;
    BoardUI _board;
    FireResourcePanelUI _firePointsPanel;
    ActionPointsPanelUI _actionPointsPanel;
    ProgressPanelUI _progressPanel;
    BattleInfoPanelUI _battleInfoPanel;
    ButtonUI _endTurnButton;
    CaptainUI _captain;
    std::list<CardUI> _cards;
    std::list<UnitUI> _units;
};
