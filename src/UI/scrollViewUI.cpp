#include "UI/scrollViewUI.h"
#include <algorithm>

ScrollViewUI::ScrollViewUI(ScrollViewUIConfig config)
    : transform(config.transform),
      background(config.background),
      _config(config) {
    transform.addChild(&content);
}

void ScrollViewUI::setContentExtentProvider(std::function<float()> extentProvider) {
    _extentProvider = std::move(extentProvider);
}

float ScrollViewUI::viewportExtent() const {
    return _config.direction == ScrollDirection::Horizontal ? _config.viewportSize.x
                                                              : _config.viewportSize.y;
}

float ScrollViewUI::contentExtent() const {
    return _extentProvider ? _extentProvider() : 0.0f;
}

void ScrollViewUI::applyOffset() {
    if (_config.direction == ScrollDirection::Horizontal)
        content.position.x = _scrollOffset;
    else
        content.position.y = _scrollOffset;
}

void ScrollViewUI::scroll(float wheelDelta) {
    float maxScroll = std::max(0.0f, contentExtent() - viewportExtent());
    _scrollOffset = std::clamp(_scrollOffset + wheelDelta * _config.scrollStep, -maxScroll, 0.0f);
    applyOffset();
}

Rectangle ScrollViewUI::viewportRect() const {
    Vector2 centre = transform.worldPosition();
    return {
        centre.x - _config.viewportSize.x / 2.0f,
        centre.y - _config.viewportSize.y / 2.0f,
        _config.viewportSize.x,
        _config.viewportSize.y
    };
}
