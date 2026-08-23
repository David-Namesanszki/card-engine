// #include "scenes/tacticalScene.h"
// #include "raylib.h"
// #include "core/battle.h"
// #include "engine/sceneManager.h"
// #include "engine/systems/renderSystem.h"
// #include "scenes/battleScene.h"

// // Core speaks in enums; the info panel displays strings. The mapping lives on
// // the UI side of the boundary.
// static const char* turnTypeLabel(TurnType turnType) {
//     return turnType == TurnType::Player ? "Player" : "Enemy";
// }

// static const char* difficultyLabel(BattleDifficultyType difficulty) {
//     switch (difficulty) {
//     case BattleDifficultyType::Minor:
//         return "Minor";
//     case BattleDifficultyType::Major:
//         return "Major";
//     case BattleDifficultyType::Boss:
//         return "Boss";
//     }
//     return "Unknown";
// }

// TacticalScene::TacticalScene(
//     Captain& captain,
//     Battle& battle,
//     SceneManager& scenes,
//     std::optional<uint32_t> demoPlayerUnitId,
//     std::optional<uint32_t> demoEnemyUnitId
// )
//     : _captain(captain),
//       _battle(battle),
//       _scenes(scenes),
//       _demoPlayerUnitId(demoPlayerUnitId),
//       _demoEnemyUnitId(demoEnemyUnitId) {
//     // ---- Configs ----
//     // Same screen regions the battle layout uses, so the shared elements
//     // (board, info panels) don't jump when the scenes switch.
//     BoardUIConfig boardConfig;
//     boardConfig.transform.position = {640.0f, 252.0f};

//     BattleInfoPanelUIConfig battleInfoPanelConfig;
//     battleInfoPanelConfig.transform.position = {64.0f, 108.0f};
//     battleInfoPanelConfig.background.texture = "assets/BattleInfoPanel.png";
//     battleInfoPanelConfig.background.size = {128.0f, 216.0f};

//     ProgressPanelUIConfig progressPanelConfig;
//     progressPanelConfig.transform.position = {1216.0f, 108.0f};
//     progressPanelConfig.background.texture = "assets/ProgressPanel.png";
//     progressPanelConfig.background.size = {128.0f, 216.0f};

//     // Roster strip: bottom centre, where the hands sit in the battle layout.
//     RosterUIConfig rosterConfig;
//     rosterConfig.grid.scrollView.transform.position = {640.0f, 612.0f};
//     rosterConfig.grid.scrollView.background.visible = false; // no art yet

//     UnitUIConfig unitConfig;

//     _ui = std::make_unique<TacticalUI>(
//         boardConfig,
//         battle.getBoard().tiles(),
//         battleInfoPanelConfig,
//         progressPanelConfig,
//         rosterConfig,
//         unitConfig
//     );

//     // TODO: portraits are placeholders until per-unit-type art (and a unit
//     // kind in core to select it) exists.
//     battle.onRosterChanged([this](Roster roster) {
//         for (const Unit& unit : roster.getUnits()) {
//             _ui->addRosterUnit(
//                 unit.getId(),
//                 unit.getName(),
//                 "assets/units/viking.png",
//                 unit.getHealth().current,
//                 unit.getAttackPower(),
//                 unit.getArmor().getDefensivePower(),
//                 0 // current armor: not yet in battle, mirrors TacticalUI::placeUnit
//             );
//         }
//         for (const Construction& construction : roster.getConstructions()) {
//             _ui->addRosterConstruction(
//                 construction.getId(),
//                 construction.getName(),
//                 "assets/boardTiles/construction_player.png",
//                 construction.getDurability().current
//             );
//         }
//     });

//     // Deployment only places units, so these two are the whole wiring.
//     // TODO: every unit is a viking until per-unit-type art (and a unit kind
//     // in core to select it) exists.
//     battle.onUnitPlaced([this](UnitPlacedEvent e) {
//         const char* texture = "assets/units/viking.png";
//         _ui->placeUnit(
//             e.unitId,
//             e.unit.getName(),
//             texture,
//             _ui->board().unitPosition(e.at),
//             e.unit.getHealth().current,
//             e.unit.getAttackPower(),
//             e.unit.getArmor().getDefensivePower()
//         );
//     });
//     battle.onBattleInfoChanged([this](BattleInfoChangedEvent e) {
//         _ui->battleInfoPanel().setDifficulty(difficultyLabel(e.info.difficulty));
//         _ui->battleInfoPanel().setWhoseTurn(turnTypeLabel(e.info.turnType));
//         _ui->battleInfoPanel().setBattleLength(e.info.turnCount);
//     });
// }

// void TacticalScene::update(float) {
//     for (const InputEvent& event : _inputSystem.getInputs()) {
//         if (event.type == InputEventType::Scrolled)
//             _ui->scrollRoster(event.scrollDelta);
//     }

//     if (IsKeyPressed(KEY_B)) {
//         // Scene-switch protocol: drop this scene's subscriptions, let the new
//         // scene wire itself, re-announce so it populates, then swap between
//         // frames. (To be folded into onExit/onEnter hooks once the shape has
//         // proven itself here.)
//         _battle.clearSubscriptions();
//         auto next = std::make_unique<BattleScene>(
//             _captain, _battle, _scenes, _demoPlayerUnitId, _demoEnemyUnitId
//         );
//         _battle.announce();
//         _scenes.requestChange(std::move(next));
//     }
// }

// void TacticalScene::render(RenderSystem& renderer) {
//     renderer.renderTactical(*_ui);

//     DrawText("Tactical phase   B start battle", 10, 10, 16, LIGHTGRAY);
// }
