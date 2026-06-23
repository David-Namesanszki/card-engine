#pragma once

#include "engine/renderObject.h"
#include "UI/anchorPoint.h"
#include "UI/configs/discardPileUIConfig.h"

class DiscardPileUI : public RenderObject {
public:
    DiscardPileUI(const int count, const DiscardPileUIConfig& config);

    void setCount(const int newCount);
    AnchorPoint getDiscardPoint() const;
private:
    int _count;
    DiscardPileUIConfig _config;
};