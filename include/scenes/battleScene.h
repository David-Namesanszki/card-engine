#pragma once

#include <cstddef>
#include <list>
#include <memory>
#include <vector>

#include "engine/scene.h"
#include "engine/systems/hitTestSystem.h"
#include "engine/systems/inputSystem.h"
#include "engine/systems/intentSystem.h"
#include "engine/systems/targetSystem.h"

#include "core/attributes/health.h"
#include "core/cardPile.h"
#include "core/entities/boardPieces/boardPiece.h"
#include "core/entities/boardTile.h"
#include "core/systems/battleResourceSystem.h"
#include "core/systems/cardFlowSystem.h"
#include "core/systems/effectSystem.h"
#include "core/systems/turnSystem.h"
#include "core/types/turnType.h"

#include "UI/boardUI.h"
#include "UI/buttonUI.h"
#include "UI/captainUI.h"
#include "UI/cardPileUI.h"
#include "UI/cardUI.h"
#include "UI/configs/cardUIConfig.h"
#include "UI/fixedSlotHandUI.h"
#include "UI/panels/actionPointsPanelUI.h"
#include "UI/panels/battleInfoPanelUI.h"
#include "UI/panels/fireResourcePanelUI.h"
#include "UI/panels/progressPanelUI.h"
#include "UI/boardPieces/unitUI.h"
#include "UI/systems/cardFlowViewSystem.h"
#include "UI/systems/battleResourceViewSystem.h"
#include "UI/systems/turnViewSystem.h"

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

// The stock 1280x720 battle layout — positions, textures and sizes for every
// widget. See the region diagram above the definition in battleScene.cpp.
BattleLayout defaultBattleLayout();

struct BattleData {
    CardPile deck;
    size_t leftHandSize = 0;
    size_t rightHandSize = 0;
    TurnType turnType = TurnType::Player;
    BattleDifficultyType difficulty;
    Health captainHealth;
    int maxActionPointCount = 0;
    int currentFirePointCount = 0;
    std::vector<std::unique_ptr<BoardPiece>> selectedBoardPieces;
    std::vector<BoardTile> boardTiles;
};

struct BattleConfig {
    // config for the engine systems
};

class BattleScene : public Scene {
  public:
    BattleScene(
        SceneManager& scenes,
        BattleData data,
        BattleLayout layout = {},
        BattleConfig config = {}
    );

    void update(float dt) override;
    void render(RenderSystem& renderer) override;

  private:
    // Debug tracing for every core event, wired independently of the widget
    // subscriptions. Silence with SetTraceLogLevel(LOG_WARNING).
    void subscribeLogging();

    // Declaration order is initialization order: _layout must stay above the
    // UI members, which read their configs out of it.
    SceneManager& _scenes; // unused until scene transitions exist; shape set now

    BattleLayout _layout;

    // engine systems
    InputSystem _inputSystem;
    HitTestSystem _hitTestSystem;

    // UI systems
    TargetSystem _targetSystem;
    IntentSystem _intentSystem;
    CardFlowViewSystem _cardFlowViewSystem;
    BattleResourceViewSystem _battleResourceViewSystem;
    TurnViewSystem _turnViewSystem;

    // core systems — none is default-constructible, so all four are
    // initialized from `data` in the constructor's init-list.
    CardFlowSystem _cardFlowSystem;
    TurnSystem _turnSystem;
    BattleResourceSystem _battleResourceSystem;
    EffectSystem _effectSystem;

    // UI elements
    BoardUI _board;
    ProgressPanelUI _progressPanel;
    ButtonUI _endTurnButton;
    CaptainUI _captain;
    std::list<UnitUI> _units;
};
