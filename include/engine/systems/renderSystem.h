#pragma once

#include "UI/battleUI.h"
#include "engine/components/transformComponent.h"
#include "engine/components/spriteComponent.h"
#include "raylib.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include "engine/components/textComponent.h"

class RenderSystem {
  public:
    ~RenderSystem();

    void renderBattle(const BattleUI& battle);

    // Darkens the whole screen, then redraws the given cards at full
    // brightness so only they stand out. Call after render(), and only while
    // targeting is in progress (this method has no notion of interaction state).
    void renderTargetOverlay(
        const BattleUI& battle,
        const std::vector<uint32_t>& targetableCardIds
    );

  private:
    // Loads `path` once and caches the GPU texture for reuse.
    const Texture2D& texture(const std::string& path);

    // Draws a sprite using its transform's world position/rotation/scale.
    void draw(const TransformComponent& transform, const SpriteComponent& sprite);
    void drawText(const TransformComponent& transform, const TextComponent& textComponent);

    void renderCardPile(const CardPileUI& cardPile);
    void renderHand(const FixedSlotHandUI& hand);
    void renderBoard(const BoardUI& board);
    void renderCard(const CardUI& card);
    void renderResourcePanel(const FireResourcePanelUI& resourcePanel);
    void renderActionPointsPanel(const ActionPointsPanelUI& actionPanel);
    void renderProgressPanel(const ProgressPanelUI& progressPanel);
    void renderBattleInfoPanel(const BattleInfoPanelUI& battleInfoPanel);
    void renderButton(const ButtonUI& button);
    void renderCaptain(const CaptainUI& captain);
    void renderUnit(const UnitUI& unit);

    std::unordered_map<std::string, Texture2D> _textures;
};
