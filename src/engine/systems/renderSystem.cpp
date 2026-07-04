#include "engine/systems/renderSystem.h"
#include <algorithm>

RenderSystem::~RenderSystem() {
    // Skip if the GL context is already gone (window closed) — the driver has
    // released the textures for us and UnloadTexture would have nothing valid.
    if (!IsWindowReady())
        return;
    for (auto& [path, tex] : _textures)
        UnloadTexture(tex);
}

const Texture2D& RenderSystem::texture(const std::string& path) {
    auto it = _textures.find(path);
    if (it == _textures.end())
        it = _textures.emplace(path, LoadTexture(path.c_str())).first;
    return it->second;
}

void RenderSystem::drawText(
    const TransformComponent& transform,
    const TextComponent& textComponent
) {
    if (!textComponent.visible)
        return;

    Vector2 pos = transform.worldPosition();
    DrawText(
        textComponent.text.c_str(),
        (int)pos.x,
        (int)pos.y,
        textComponent.fontSize,
        textComponent.color
    );
}

void RenderSystem::draw(const TransformComponent& transform, const SpriteComponent& sprite) {
    if (!sprite.visible)
        return;

    const Texture2D& tex = texture(sprite.texture);

    float scale = transform.worldScale();
    float w =
        sprite.size.x * scale; // draw at the sprite's target size, not the texture's native size
    float h = sprite.size.y * scale;
    Vector2 pos = transform.worldPosition();

    Rectangle source = {0.0f, 0.0f, (float)tex.width, (float)tex.height};
    Rectangle dest = {pos.x, pos.y, w, h};
    Vector2 origin = {w * 0.5f, h * 0.5f}; // rotate/position about the centre

    DrawTexturePro(tex, source, dest, origin, transform.worldRotation() * RAD2DEG, sprite.color);
}

void RenderSystem::renderBattle(const BattleUI& battle) {
    // Backgrounds first, then cards on top.
    draw(battle.board().transform, battle.board().sprite);
    for (const auto& tile : battle.board().tiles())
        draw(tile.transform, tile.sprite);

    const CaptainUI& captain = battle.captain();
    draw(captain.transform, captain.sprite);
    draw(captain.healthPip.transform, captain.healthPip.sprite);

    for (const auto& unit : battle.units()) {
        draw(unit.transform, unit.sprite);
        draw(unit.healthPip.transform, unit.healthPip.sprite);
        draw(unit.attackPowerPip.transform, unit.attackPowerPip.sprite);
        draw(unit.defensivePowerPip.transform, unit.defensivePowerPip.sprite);
    }

    draw(battle.drawPile().transform, battle.drawPile().sprite);
    draw(battle.discardPile().transform, battle.discardPile().sprite);
    draw(battle.leftHand().transform, battle.leftHand().sprite);
    draw(battle.rightHand().transform, battle.rightHand().sprite);

    for (const auto& card : battle.cards())
        draw(card.transform, card.sprite);

    draw(battle.firePanel().transform, battle.firePanel().background);

    draw(battle.actionPointsPanel().transform, battle.actionPointsPanel().sprite);
    for (const auto& pip : battle.actionPointsPanel().pips())
        draw(pip.transform, pip.sprite);

    const ProgressPanelUI& progress = battle.progressPanel();
    draw(progress.transform, progress.background);
    draw(progress.daysPip.transform, progress.daysPip.sprite);
    draw(progress.raidSectionPip.transform, progress.raidSectionPip.sprite);
    draw(progress.barkOrePip.transform, progress.barkOrePip.sprite);
    draw(progress.bloodSapPip.transform, progress.bloodSapPip.sprite);
    draw(progress.heartWoodPip.transform, progress.heartWoodPip.sprite);

    const BattleInfoPanelUI& info = battle.battleInfoPanel();
    draw(info.transform, info.background);
    draw(info.difficultyPip.transform, info.difficultyPip.sprite);
    draw(info.whoseTurnPip.transform, info.whoseTurnPip.sprite);
    draw(info.battleLengthPip.transform, info.battleLengthPip.sprite);

    draw(battle.endTurnButton().transform, battle.endTurnButton().sprite);

    drawText(battle.drawPile().countText.transform, battle.drawPile().countText.text);
    drawText(battle.discardPile().countText.transform, battle.discardPile().countText.text);
    drawText(battle.firePanel().fireCountText.transform, battle.firePanel().fireCountText.text);

    drawText(progress.currentDaysText.transform, progress.currentDaysText.text);
    drawText(progress.maxDaysText.transform, progress.maxDaysText.text);
    drawText(progress.raidSectionCountText.transform, progress.raidSectionCountText.text);
    drawText(progress.barkOreCountText.transform, progress.barkOreCountText.text);
    drawText(progress.bloodSapCountText.transform, progress.bloodSapCountText.text);
    drawText(progress.heartWoodCountText.transform, progress.heartWoodCountText.text);

    drawText(info.difficultyText.transform, info.difficultyText.text);
    drawText(info.whoseTurnText.transform, info.whoseTurnText.text);
    drawText(info.battleLengthText.transform, info.battleLengthText.text);

    drawText(captain.healthText.transform, captain.healthText.text);
    for (const auto& unit : battle.units()) {
        drawText(unit.healthText.transform, unit.healthText.text);
        drawText(unit.attackPowerText.transform, unit.attackPowerText.text);
        drawText(unit.defensivePowerText.transform, unit.defensivePowerText.text);
    }
}

void RenderSystem::renderTargetOverlay(
    const BattleUI& battle,
    const std::vector<uint32_t>& targetableCardIds
) {
    // Darken everything drawn so far this frame...
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 150});

    // ...then redraw only the targetable cards on top, so they pop back to
    // full brightness against the darkened background.
    for (const auto& card : battle.cards()) {
        bool targetable =
            std::find(targetableCardIds.begin(), targetableCardIds.end(), card.id()) !=
            targetableCardIds.end();
        if (targetable)
            draw(card.transform, card.sprite);
    }
}
