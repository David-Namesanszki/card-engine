#include "engine/sceneManager.h"

void SceneManager::requestChange(std::unique_ptr<Scene> next) {
    _pending = std::move(next);
}

void SceneManager::applyPending() {
    if (_pending)
        _current = std::move(_pending);
}

void SceneManager::update(float dt) {
    if (_current)
        _current->update(dt);
}

void SceneManager::render(RenderSystem& renderer) {
    if (_current)
        _current->render(renderer);
}
