#pragma once

#include "engine/hitBoxComponent.h"
#include "engine/spriteComponent.h"
#include "engine/transformComponent.h"
#include "UI/anchorPoint.h"
#include "UI/configs/cardPileUIConfig.h"

class CardPileUI {
  public:
    CardPileUI(CardPileUIConfig config = {});

    SpriteComponent sprite;
    TransformComponent transform;
    HitBoxComponent hitbox;
    int count;

    const AnchorPoint& getAnchorPoint() const;

  private:
    AnchorPoint _anchorPoint;
    CardPileUIConfig _config;
};