#pragma once

#include "engine/animator.h"
#include "UI/rightHandUI.h"
#include "UI/leftHandUI.h"
#include "UI/discardPileUI.h"
#include "UI/drawPileUI.h"
#include "UI/configs/battleUIConfig.h"
#include <list>

class BattleUI {
  public:
    BattleUI(
        Animator& animator,
        BattleUIConfig config = {},
        LeftHandUIConfig leftHandConfig = {},
        RightHandUIConfig rightHandConfig = {},
        DiscardPileUIConfig discardPileConfig = {},
        DrawPileUIConfig drawPileConfig = {});

    void drawCard();
    void discardFromLeftHand(uint32_t cardId);
    void discardFromRightHand(uint32_t cardId);
    void reshuffleDiscardIntoDraw();
    void draw() const;

  private:
    // A single flourish representing the discard sweeping back into the deck.
    void playReshuffleEffect(Vector2 from, Vector2 to);

    Animator& _animator;
    BattleUIConfig _config;
    DrawPileUI _drawPile;
    DiscardPileUI _discardPile;
    LeftHandUI _leftHand;
    RightHandUI _rightHand;

    // Transient visuals owned by BattleUI; live only while their animation runs.
    std::list<TransformComponent> _reshuffleGhosts;
};
