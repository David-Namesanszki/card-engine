#pragma once

#include "engine/systems/animationSystem.h"
#include "UI/fixedSlotHandUI.h"
#include "UI/cardPileUI.h"
#include "UI/configs/battleUIConfig.h"
#include <list>
#include <string>
#include <cstdint>
#include "UI/cardUI.h"
#include "engine/eventBus.h"
#include "UI/boardUI.h"
#include "UI/configs/boardUIConfig.h"
#include "UI/panels/fireResourcePanelUI.h"
#include "UI/configs/fireResourcePanelUIConfig.h"
#include "UI/panels/actionPointsPanelUI.h"
#include "UI/configs/actionPointsPanelUIConfig.h"
#include "UI/panels/progressPanelUI.h"
#include "UI/configs/progressPanelUIConfig.h"
#include "UI/panels/battleInfoPanelUI.h"
#include "UI/configs/battleInfoPanelUIConfig.h"
#include "UI/buttonUI.h"
#include "UI/configs/buttonUIConfig.h"
#include "UI/captainUI.h"
#include "UI/unitUI.h"

class BattleUI {
  public:
    BattleUI(
        BattleUIConfig config = {},
        BoardUIConfig boardConfig = {},
        CaptainUIConfig captainConfig = {},
        FireResourcePanelUIConfig firePanelConfig = {},
        ActionPointsPanelUIConfig actionPointsConfig = {},
        ProgressPanelUIConfig progressPanelConfig = {},
        BattleInfoPanelUIConfig battleInfoPanelConfig = {},
        ButtonUIConfig endTurnButtonConfig = {},
        HandUIConfig leftHandConfig = {},
        HandUIConfig rightHandConfig = {},
        CardPileUIConfig discardPileConfig = {},
        CardPileUIConfig drawPileConfig = {}
    );

    TransformComponent transform;
    SpriteComponent sprite;

    void refillDrawPile(int drawPileSize, int discardPileSize);
    void drawCard(uint32_t cardId, std::string texture, int drawPileSize);
    void discardFromLeftHand(uint32_t cardId, int discardPileSize);
    void discardFromRightHand(uint32_t cardId, int discardPileSize);
    void transferCardToRight(uint32_t cardId);
    void setFireCount(int fireCount);
    void setActionPointsSpent(int spentCount);
    void setDifficulty(const std::string& difficulty);
    void setWhoseTurn(const std::string& whoseTurn);
    void setBattleLength(int battleLength);
    void setCaptainHealth(int current, int max);

    const CardPileUI& drawPile() const {
        return _drawPile;
    };
    const CardPileUI& discardPile() const {
        return _discardPile;
    }
    const FixedSlotHandUI& leftHand() const {
        return _leftHand;
    }
    const FixedSlotHandUI& rightHand() const {
        return _rightHand;
    }
    const std::list<CardUI>& cards() const {
        return _cards;
    }
    const BoardUI& board() const {
        return _board;
    }
    const FireResourcePanelUI& firePanel() const {
        return _firePanel;
    }
    const ActionPointsPanelUI& actionPointsPanel() const {
        return _actionPointsPanel;
    }
    const ProgressPanelUI& progressPanel() const {
        return _progressPanel;
    }
    const BattleInfoPanelUI& battleInfoPanel() const {
        return _battleInfoPanel;
    }
    const ButtonUI& endTurnButton() const {
        return _endTurnButton;
    }
    const CaptainUI& captain() const {
        return _captain;
    }
    const std::list<UnitUI>& units() const {
        return _units;
    }

  private:
    void reorganizeHand(const FixedSlotHandUI& hand);
    void addCard(uint32_t cardId, std::string texture);
    CardUI takeCard(uint32_t cardId);

    BattleUIConfig _config;

    CardPileUI _drawPile;
    CardPileUI _discardPile;
    FixedSlotHandUI _leftHand;
    FixedSlotHandUI _rightHand;
    BoardUI _board;
    std::list<CardUI> _cards;
    FireResourcePanelUI _firePanel;
    ActionPointsPanelUI _actionPointsPanel;
    ProgressPanelUI _progressPanel;
    BattleInfoPanelUI _battleInfoPanel;
    ButtonUI _endTurnButton;
    CaptainUI _captain;
    std::list<UnitUI> _units;
};
