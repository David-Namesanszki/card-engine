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

    // Poor-man's outline: the text drawn at the eight surrounding offsets in
    // the outline colour, then the real text on top.
    int t = textComponent.outlineThickness;
    if (t > 0) {
        for (int dx = -t; dx <= t; dx += t)
            for (int dy = -t; dy <= t; dy += t) {
                if (dx == 0 && dy == 0)
                    continue;
                DrawText(
                    textComponent.text.c_str(),
                    (int)pos.x + dx,
                    (int)pos.y + dy,
                    textComponent.fontSize,
                    textComponent.outlineColor
                );
            }
    }

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
    renderBoard(battle.board());

    renderCaptain(battle.captain());
    for (const auto& unit : battle.units())
        renderUnit(unit);

    renderCardPile(battle.drawPile());
    renderCardPile(battle.discardPile());
    renderHand(battle.leftHand());
    renderHand(battle.rightHand());

    for (const auto& card : battle.cards())
        renderCard(card);

    renderResourcePanel(battle.firePanel());
    renderActionPointsPanel(battle.actionPointsPanel());
    renderProgressPanel(battle.progressPanel());
    renderBattleInfoPanel(battle.battleInfoPanel());

    renderButton(battle.endTurnButton());
}

void RenderSystem::renderBoard(const BoardUI& board) {
    draw(board.transform, board.sprite);
    for (const auto& tile : board.tiles())
        draw(tile.transform, tile.sprite);
}

void RenderSystem::renderCaptain(const CaptainUI& captain) {
    draw(captain.transform, captain.sprite);
    draw(captain.healthPip.transform, captain.healthPip.sprite);
    drawText(captain.healthText.transform, captain.healthText.text);
}

void RenderSystem::renderUnit(const UnitUI& unit) {
    draw(unit.transform, unit.sprite);
    draw(unit.namePip.transform, unit.namePip.sprite);
    drawText(unit.nameText.transform, unit.nameText.text);
    draw(unit.healthPip.transform, unit.healthPip.sprite);
    draw(unit.attackPowerPip.transform, unit.attackPowerPip.sprite);
    draw(unit.defensivePowerPip.transform, unit.defensivePowerPip.sprite);
    draw(unit.armorPip.transform, unit.armorPip.sprite);

    drawText(unit.healthText.transform, unit.healthText.text);
    drawText(unit.attackPowerText.transform, unit.attackPowerText.text);
    drawText(unit.defensivePowerText.transform, unit.defensivePowerText.text);
    drawText(unit.armorText.transform, unit.armorText.text);
}

void RenderSystem::renderCardPile(const CardPileUI& cardPile) {
    draw(cardPile.transform, cardPile.sprite);
    drawText(cardPile.countText.transform, cardPile.countText.text);
}

void RenderSystem::renderHand(const FixedSlotHandUI& hand) {
    draw(hand.transform, hand.sprite);
}

void RenderSystem::renderCard(const CardUI& card) {
    draw(card.transform, card.sprite);
}

void RenderSystem::renderButton(const ButtonUI& button) {
    draw(button.transform, button.sprite);
}

void RenderSystem::renderResourcePanel(const FireResourcePanelUI& resourcePanel) {
    draw(resourcePanel.transform, resourcePanel.background);
    draw(resourcePanel.firePip.transform, resourcePanel.firePip.sprite);
    drawText(resourcePanel.fireCountText.transform, resourcePanel.fireCountText.text);
}

void RenderSystem::renderActionPointsPanel(const ActionPointsPanelUI& actionPanel) {
    draw(actionPanel.transform, actionPanel.sprite);
    for (const auto& pip : actionPanel.pips())
        draw(pip.transform, pip.sprite);
}

void RenderSystem::renderProgressPanel(const ProgressPanelUI& progressPanel) {
    draw(progressPanel.transform, progressPanel.background);
    draw(progressPanel.daysPip.transform, progressPanel.daysPip.sprite);
    draw(progressPanel.raidSectionPip.transform, progressPanel.raidSectionPip.sprite);
    draw(progressPanel.barkOrePip.transform, progressPanel.barkOrePip.sprite);
    draw(progressPanel.bloodSapPip.transform, progressPanel.bloodSapPip.sprite);
    draw(progressPanel.heartWoodPip.transform, progressPanel.heartWoodPip.sprite);

    drawText(progressPanel.currentDaysText.transform, progressPanel.currentDaysText.text);
    drawText(progressPanel.maxDaysText.transform, progressPanel.maxDaysText.text);
    drawText(progressPanel.raidSectionCountText.transform, progressPanel.raidSectionCountText.text);
    drawText(progressPanel.barkOreCountText.transform, progressPanel.barkOreCountText.text);
    drawText(progressPanel.bloodSapCountText.transform, progressPanel.bloodSapCountText.text);
    drawText(progressPanel.heartWoodCountText.transform, progressPanel.heartWoodCountText.text);
}

void RenderSystem::renderBattleInfoPanel(const BattleInfoPanelUI& battleInfoPanel) {
    draw(battleInfoPanel.transform, battleInfoPanel.background);
    draw(battleInfoPanel.difficultyPip.transform, battleInfoPanel.difficultyPip.sprite);
    draw(battleInfoPanel.whoseTurnPip.transform, battleInfoPanel.whoseTurnPip.sprite);
    draw(battleInfoPanel.battleLengthPip.transform, battleInfoPanel.battleLengthPip.sprite);

    drawText(battleInfoPanel.difficultyText.transform, battleInfoPanel.difficultyText.text);
    drawText(battleInfoPanel.whoseTurnText.transform, battleInfoPanel.whoseTurnText.text);
    drawText(battleInfoPanel.battleLengthText.transform, battleInfoPanel.battleLengthText.text);
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
