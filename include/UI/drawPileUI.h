#pragma once

#include "engine/renderObject.h"
#include "UI/anchorPoint.h"
#include "UI/configs/drawPileUIConfig.h"

class DrawPileUI : public RenderObject {
public:
    DrawPileUI(const int count, const DrawPileUIConfig& config);

    void setCount(const int newCount);
    AnchorPoint getDrawPoint() const;
private:
    int _count;
    DrawPileUIConfig _config;
};