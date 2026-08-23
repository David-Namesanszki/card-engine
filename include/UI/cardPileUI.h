#pragma once

#include "engine/components/hitBoxComponent.h"
#include "engine/components/spriteComponent.h"
#include "engine/components/transformComponent.h"
#include "UI/anchorPoint.h"
#include "UI/textUI.h"
#include "UI/configs/cardPileUIConfig.h"

class CardPileUI {
  public:
    CardPileUI(CardPileUIConfig config = {}, size_t size = 0);

    void setCount(int count);

    SpriteComponent sprite;
    TransformComponent transform;
    HitBoxComponent hitbox;
    TextUI countText;

    const AnchorPoint& getAnchorPoint() const;

  private:
    AnchorPoint _anchorPoint;
    CardPileUIConfig _config;
};