#include "engine/components/transformComponent.h"
#include <algorithm>

void TransformComponent::addChild(TransformComponent* child) {
    if (child->parent)
        child->parent->removeChild(child); // detach first
    children.push_back(child);
    child->parent = this;
}

void TransformComponent::removeChild(TransformComponent* child) {
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end())
        children.erase(it);
    child->parent = nullptr;
}

TransformComponent::~TransformComponent() {
    if (parent)
        parent->removeChild(this); // tell parent I'm gone
    for (auto* c : children)
        c->parent = nullptr; // orphan my children, don't leave them pointing at freed memory
}
