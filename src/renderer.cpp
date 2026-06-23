#include "engine/renderer.h"
#include <algorithm>

Renderer::Renderer() {}

uint32_t Renderer::create(RenderObject obj) {
    uint32_t id = _nextId++;
    _objects[id] = std::move(obj);
    return id;
}

void Renderer::destroy(uint32_t id) {
    _objects.erase(id);
}

RenderObject& Renderer::get(uint32_t id) {
    return _objects.at(id);
}

void Renderer::draw(const std::vector<const RenderObject*>& extras) {
    ClearBackground({20, 30, 20, 255});
    for (const RenderObject* obj : sorted(extras))
        drawObject(*obj);
}

std::vector<const RenderObject*> Renderer::sorted(const std::vector<const RenderObject*>& extras) const {
    std::vector<const RenderObject*> result;
    for (const auto& [id, obj] : _objects)
        if (obj.visible) result.push_back(&obj);
    for (const RenderObject* obj : extras)
        if (obj && obj->visible) result.push_back(obj);
    std::sort(result.begin(), result.end(),
        [](const RenderObject* a, const RenderObject* b) { return a->layer < b->layer; });
    return result;
}

void Renderer::drawObject(const RenderObject& obj) {
    Vector2 size = {obj.size.x * obj.scale, obj.size.y * obj.scale};
    DrawRectangleV(obj.position, size, obj.color);
    DrawRectangleLinesEx({obj.position.x, obj.position.y, size.x, size.y}, 1.0f, WHITE);
    if (!obj.text.empty())
        DrawText(obj.text.c_str(), obj.position.x + 5, obj.position.y + size.y / 2 - 10, 20, WHITE);
}
