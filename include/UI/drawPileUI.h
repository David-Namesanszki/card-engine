#pragma once

#include "UI/anchorPoint.h"
#include "UI/cardUI.h"
#include "engine/spriteComponent.h"
#include "engine/transformComponent.h"
#include "engine/hitboxComponent.h"
#include "UI/configs/drawPileUIConfig.h"
#include <list>

class DrawPileUI {
  public:
    DrawPileUI(DrawPileUIConfig config = {});

    void add(CardUI card);
    CardUI removeTopCard();

    bool empty() const { return _cards.empty(); }
    int count() const { return static_cast<int>(_cards.size()); }
    const std::list<CardUI>& cards() const { return _cards; }
    const CardUI* top() const { return _cards.empty() ? nullptr : &_cards.back(); }

    SpriteComponent sprite;
    TransformComponent transform;
    HitBoxComponent hitbox;
    AnchorPoint drawPoint;

  private:
    DrawPileUIConfig _config;
    std::list<CardUI> _cards;
};