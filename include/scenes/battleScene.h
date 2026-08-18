#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include "engine/scene.h"
#include "UI/battleUI.h"
#include "engine/systems/inputSystem.h"
#include "engine/systems/hitTestSystem.h"
#include "engine/systems/targetSystem.h"
#include "engine/systems/intentSystem.h"

class Battle;
class Captain;
class SceneManager;

// The combat view: owns the battle UI composition and the battle-specific
// input systems, and wires itself to the core's events in its constructor.
// The core Battle and Captain live above the scene and outlive it.
class BattleScene : public Scene {
  public:
    BattleScene(
        Captain& captain,
        Battle& battle,
        SceneManager& scenes,
        std::optional<uint32_t> demoPlayerUnitId = {},
        std::optional<uint32_t> demoEnemyUnitId = {}
    );

    void update(float dt) override;
    void render(RenderSystem& renderer) override;

  private:
    Battle& _battle;
    SceneManager& _scenes; // unused until scene transitions exist; shape set now

    // Heap-allocated for a stable address: the composition's panels parent
    // their own child transforms, so it must never be moved or copied.
    std::unique_ptr<BattleUI> _ui;

    InputSystem _inputSystem;
    HitTestSystem _hitTestSystem;
    TargetSystem _targetSystem;
    IntentSystem _intentSystem;
    std::optional<uint32_t> _playedCardId;

    // Demo scaffolding: the pre-placed lane units the debug keys drive.
    std::optional<uint32_t> _demoPlayerUnitId;
    std::optional<uint32_t> _demoEnemyUnitId;
};
