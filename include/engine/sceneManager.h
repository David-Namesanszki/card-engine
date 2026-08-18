#pragma once

#include <memory>
#include "engine/scene.h"

class SceneManager {
  public:
    // Stores the next scene; applyPending() swaps it in between frames — a
    // scene must never be destroyed while its own update() is on the stack.
    void requestChange(std::unique_ptr<Scene> next);
    void applyPending();

    void update(float dt);
    void render(RenderSystem& renderer);

  private:
    std::unique_ptr<Scene> _current;
    std::unique_ptr<Scene> _pending;
};
