#pragma once

#include <functional>
#include "raylib.h"
#include "engine/components/transformComponent.h"
#include "engine/components/spriteComponent.h"
#include "UI/configs/scrollViewUIConfig.h"

// Reusable scrolling viewport. It owns the clip rectangle and the content
// offset math along one axis and knows nothing about what it scrolls: parent
// your items' transforms to `content`, then register a callback that answers
// "how much content is there right now". The extent is asked for fresh every
// time it's needed, never cached, so it can never drift out of sync with
// what's actually under `content`.
//
// Meant to be embedded as a direct member of the widget that uses it (see
// ScrollableGridUI) — like the panel widgets, it is never copied because it
// never leaves the member slot it was constructed into. If the provider is a
// lambda capturing `this` from that owner, the owner must honour the same
// stable-address contract.
class ScrollViewUI {
  public:
    ScrollViewUI(ScrollViewUIConfig config = {});

    void setContentExtentProvider(std::function<float()> extentProvider);

    void scroll(float wheelDelta);
    Rectangle viewportRect() const;

    TransformComponent transform;
    SpriteComponent background;
    TransformComponent content; // child of transform; scrolling moves this

  private:
    void applyOffset();
    float viewportExtent() const;
    float contentExtent() const;

    ScrollViewUIConfig _config;
    std::function<float()> _extentProvider;
    float _scrollOffset = 0.0f;
};
