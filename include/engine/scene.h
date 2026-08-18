#pragma once

class RenderSystem;

// One view of the game (menu, map, tactical phase, battle...). A scene owns
// its UI composition, its scene-specific systems and its event wiring; state
// that outlives the scene (core objects, the texture cache) lives above it
// and is passed in by reference.
class Scene {
  public:
    virtual ~Scene() = default;

    virtual void update(float dt) = 0;
    virtual void render(RenderSystem& renderer) = 0;
};
