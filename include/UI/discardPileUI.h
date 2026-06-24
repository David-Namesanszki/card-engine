#pragma once

#include "engine/animator.h"
#include "engine/hitBoxComponent.h"
#include "engine/spriteComponent.h"
#include "engine/transformComponent.h"
#include "UI/anchorPoint.h"
#include "UI/cardUI.h"
#include "UI/configs/discardPileUIConfig.h"
#include <list>

class DiscardPileUI {
  public:
    DiscardPileUI(Animator& animator, DiscardPileUIConfig config = {});

    void add(CardUI card);
    CardUI removeTopCard();

    bool empty() const { return _cards.empty(); }
    int count() const { return static_cast<int>(_cards.size()); }
    const std::list<CardUI>& cards() const { return _cards; }
    const CardUI* top() const { return _cards.empty() ? nullptr : &_cards.back(); }

    SpriteComponent sprite;
    TransformComponent transform;
    HitBoxComponent hitbox;
    AnchorPoint discardPoint;

  private:
    Animator& _animator;
    DiscardPileUIConfig _config;
    std::list<CardUI> _cards;
};