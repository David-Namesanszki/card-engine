#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include "engine/scene.h"
#include "engine/systems/inputSystem.h"
#include "UI/tacticalUI.h"

class Battle;
class Captain;
class SceneManager;

// The deployment view before combat: the board framed by the info panels.
// Units only get placed here — no combat events are wired because none can
// happen during deployment. Pressing B hands the battle over to BattleScene.
class TacticalScene : public Scene {
  public:
    TacticalScene(
        Captain& captain,
        Battle& battle,
        SceneManager& scenes,
        std::optional<uint32_t> demoPlayerUnitId = {},
        std::optional<uint32_t> demoEnemyUnitId = {}
    );

    void update(float dt) override;
    void render(RenderSystem& renderer) override;

  private:
    Captain& _captain; // only forwarded to the BattleScene this scene spawns
    Battle& _battle;
    SceneManager& _scenes;

    // Heap-allocated for a stable address: the composition's panels parent
    // their own child transforms, so it must never be moved or copied.
    std::unique_ptr<TacticalUI> _ui;

    InputSystem _inputSystem;

    // Demo scaffolding, forwarded to BattleScene's debug keys.
    std::optional<uint32_t> _demoPlayerUnitId;
    std::optional<uint32_t> _demoEnemyUnitId;
};
