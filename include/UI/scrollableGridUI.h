#pragma once

#include "engine/components/transformComponent.h"
#include "UI/scrollViewUI.h"
#include "UI/configs/scrollableGridUIConfig.h"

class ScrollableGridUI {
  public:
    ScrollableGridUI(ScrollableGridUIConfig config = {});

    void addItem(TransformComponent& itemTransform);

    void scroll(float wheelDelta) {
        _scrollView.scroll(wheelDelta);
    }
    const ScrollViewUI& scrollView() const {
        return _scrollView;
    }

  private:
    float mainAxisExtent() const;

    ScrollableGridUIConfig _config;
    ScrollViewUI _scrollView;
    int _itemCount = 0;
};
