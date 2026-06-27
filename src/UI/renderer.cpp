#include "UI/renderer.h"

Renderer::~Renderer() {
    // Skip if the GL context is already gone (window closed) — the driver has
    // released the textures for us and UnloadTexture would have nothing valid.
    if (!IsWindowReady())
        return;
    for (auto& [path, tex] : _textures)
        UnloadTexture(tex);
}

const Texture2D& Renderer::texture(const std::string& path) {
    auto it = _textures.find(path);
    if (it == _textures.end())
        it = _textures.emplace(path, LoadTexture(path.c_str())).first;
    return it->second;
}

void Renderer::draw(const TransformComponent& transform, const SpriteComponent& sprite) {
    if (!sprite.visible)
        return;

    const Texture2D& tex = texture(sprite.texture);

    float   scale = transform.worldScale();
    float   w = tex.width * scale;
    float   h = tex.height * scale;
    Vector2 pos = transform.worldPosition();

    Rectangle source = {0.0f, 0.0f, (float)tex.width, (float)tex.height};
    Rectangle dest = {pos.x, pos.y, w, h};
    Vector2   origin = {w * 0.5f, h * 0.5f}; // rotate/position about the centre

    DrawTexturePro(tex, source, dest, origin, transform.worldRotation() * RAD2DEG, sprite.color);
}

void Renderer::render(const BattleUI& battle) {
    // Backgrounds first, then cards on top.
    draw(battle.drawPile().transform, battle.drawPile().sprite);
    draw(battle.discardPile().transform, battle.discardPile().sprite);
    draw(battle.leftHand().transform, battle.leftHand().sprite);
    draw(battle.rightHand().transform, battle.rightHand().sprite);

    for (const auto& card : battle.cards())
        draw(card.transform, card.sprite);
}
