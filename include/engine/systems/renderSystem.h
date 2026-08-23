#pragma once

#include "engine/components/spriteComponent.h"
#include "engine/components/textComponent.h"
#include "engine/components/transformComponent.h"
#include "raylib.h"
#include <string>
#include <unordered_map>

// Every widget below is taken by const reference only, so a forward
// declaration is all this header needs. The implementation includes the real
// headers; consumers already have the widget they are asking us to draw.
class ActionPointsPanelUI;
class BattleInfoPanelUI;
class BoardUI;
class ButtonUI;
class CaptainUI;
class CardPileUI;
class CardUI;
class ConstructionUI;
class FireResourcePanelUI;
class FixedSlotHandUI;
class ProgressPanelUI;
class RosterUI;
class TacticalUI;
class UnitUI;

class RenderSystem {
  public:
    ~RenderSystem();

    void renderTactical(const TacticalUI& tactical);

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
    void renderConstruction(const ConstructionUI& construction);
    void renderRoster(const RosterUI& roster);

    std::unordered_map<std::string, Texture2D> _textures;
};
