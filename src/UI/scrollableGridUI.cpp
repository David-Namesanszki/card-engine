#include "UI/scrollableGridUI.h"

namespace {
// The scroll axis must be the one that actually grows: rows when wrapping,
// columns (i.e. the single row itself) when not. Deriving it here rather
// than trusting the config prevents the two settings disagreeing.
ScrollViewUIConfig withDirectionFromWrap(ScrollViewUIConfig config, int wrapCount) {
    config.direction = wrapCount > 0 ? ScrollDirection::Vertical : ScrollDirection::Horizontal;
    return config;
}
} // namespace

ScrollableGridUI::ScrollableGridUI(ScrollableGridUIConfig config)
    : _config(config),
      _scrollView(withDirectionFromWrap(config.scrollView, config.wrapCount)) {
    _scrollView.setContentExtentProvider([this] { return mainAxisExtent(); });
}

void ScrollableGridUI::addItem(TransformComponent& itemTransform) {
    bool wraps = _config.wrapCount > 0;
    int col = wraps ? _itemCount % _config.wrapCount : _itemCount;
    int row = wraps ? _itemCount / _config.wrapCount : 0;

    Vector2 position;
    if (wraps) {
        // Columns are capped at wrapCount, so they're centred; rows keep
        // growing, so they're edge-anchored to the viewport and scroll.
        float gridWidth = (float)_config.wrapCount * _config.itemOffset.x;
        position.x = -gridWidth / 2.0f + _config.itemOffset.x * (col + 0.5f);
        position.y =
            -_config.scrollView.viewportSize.y / 2.0f + _config.itemOffset.y * (row + 0.5f);
    } else {
        // A single row that never wraps: it keeps growing, so it's
        // edge-anchored to the viewport and scrolls; there is only ever one
        // row, so it's simply centred (row == 0 always).
        position.x =
            -_config.scrollView.viewportSize.x / 2.0f + _config.itemOffset.x * (col + 0.5f);
        position.y = 0.0f;
    }

    itemTransform.position = position;
    _scrollView.content.addChild(&itemTransform);
    ++_itemCount;
}

float ScrollableGridUI::mainAxisExtent() const {
    if (_config.wrapCount > 0) {
        int rows = (_itemCount + _config.wrapCount - 1) / _config.wrapCount; // ceil
        return (float)rows * _config.itemOffset.y;
    }
    return (float)_itemCount * _config.itemOffset.x;
}
