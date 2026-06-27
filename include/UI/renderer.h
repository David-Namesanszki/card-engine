#pragma once

#include "UI/battleUI.h"
#include "engine/transformComponent.h"
#include "engine/spriteComponent.h"
#include "raylib.h"
#include <string>
#include <unordered_map>

class Renderer {
  public:
    ~Renderer();

    void render(const BattleUI& battle);

  private:
    // Loads `path` once and caches the GPU texture for reuse.
    const Texture2D& texture(const std::string& path);

    // Draws a sprite using its transform's world position/rotation/scale.
    void draw(const TransformComponent& transform, const SpriteComponent& sprite);

    std::unordered_map<std::string, Texture2D> _textures;
};
