#include "UI/battleUI.h"
#include "raylib.h"
#include "engine/easing.h"
#include <algorithm>
#include <random>
#include <vector>

BattleUI::BattleUI(
    BattleUIConfig config,
    BoardUIConfig boardConfig,
    const std::vector<BoardTile>& boardTiles,
    CaptainUIConfig captainConfig,
    FireResourcePanelUIConfig firePanelConfig,
    ActionPointsPanelUIConfig actionPointsConfig,
    ProgressPanelUIConfig progressPanelConfig,
    BattleInfoPanelUIConfig battleInfoPanelConfig,
    ButtonUIConfig endTurnButtonConfig,
    HandUIConfig leftHandConfig,
    HandUIConfig rightHandConfig,
    CardPileUIConfig discardPileConfig,
    CardPileUIConfig drawPileConfig,
    UnitUIConfig unitConfig
)
    : _config(config),
      _unitConfig(unitConfig),
      _board(boardConfig, boardTiles),
      _captain(captainConfig),
      _drawPile(drawPileConfig),
      _discardPile(discardPileConfig),
      _leftHand(leftHandConfig),
      _rightHand(rightHandConfig),
      _firePanel(firePanelConfig),
      _actionPointsPanel(actionPointsConfig),
      _progressPanel(progressPanelConfig),
      _battleInfoPanel(battleInfoPanelConfig),
      _endTurnButton(endTurnButtonConfig) {
}

#pragma region Domain

void startBattle(
    Health captainHealth,
    Deck deck,
    Board board,
    std::vector<Unit> units,
    std::vector<Construction> constructions,
    ActionPoint actionPoint,
    BattleInfo battleInfo
) {
    _captain
}

void BattleUI::refillDrawPile(int drawPileSize, int discardPileSize) {
    _drawPile.setCount(drawPileSize);
    _discardPile.setCount(discardPileSize);
}

void BattleUI::setFireCount(int fireCount) {
    _firePanel.setFireCount(fireCount);
}

void BattleUI::setActionPoints(int current, int max) {
    _actionPointsPanel.setActionPoints(current, max);
}

void BattleUI::setDifficulty(const std::string& difficulty) {
    _battleInfoPanel.setDifficulty(difficulty);
}

void BattleUI::setWhoseTurn(const std::string& whoseTurn) {
    _battleInfoPanel.setWhoseTurn(whoseTurn);
}

void BattleUI::setBattleLength(int battleLength) {
    _battleInfoPanel.setBattleLength(battleLength);
}

void BattleUI::setCaptainHealth(int current, int max) {
    _captain.setHealth(current, max);
}

void BattleUI::placeUnit(
    uint32_t unitId,
    const std::string& name,
    const std::string& texture,
    Vector2 position,
    int health,
    int attackPower,
    int defensivePower
) {
    UnitUI& unit = _units.emplace_back(unitId, _unitConfig);
    unit.sprite.texture = texture;
    unit.setName(name);
    unit.setHealth(health);
    unit.setAttackPower(attackPower);
    unit.setDefensivePower(defensivePower);
    unit.setArmor(0);
    unit.transform.position = position;
}

void BattleUI::moveUnit(uint32_t unitId, Vector2 position) {
    if (UnitUI* unit = findUnit(unitId))
        unit->transform.position = position;
}

void BattleUI::setUnitHealth(uint32_t unitId, int health) {
    if (UnitUI* unit = findUnit(unitId))
        unit->setHealth(health);
}

void BattleUI::setUnitArmor(uint32_t unitId, int armor) {
    if (UnitUI* unit = findUnit(unitId))
        unit->setArmor(armor);
}

void BattleUI::removeUnit(uint32_t unitId) {
    _units.remove_if([unitId](const UnitUI& unit) { return unit.id() == unitId; });
}

void BattleUI::transferCardToRight(uint32_t cardId) {
    _leftHand.removeCardId(cardId);
    reorganizeHand(_leftHand);
    _rightHand.addCardId(cardId, 0);
    reorganizeHand(_rightHand);
}

void BattleUI::drawCard(uint32_t cardId, std::string texture, int drawPileSize) {
    addCard(cardId, texture);
    _drawPile.setCount(drawPileSize);
    _leftHand.addCardId(cardId, 0);
    reorganizeHand(_leftHand);
}

void BattleUI::discardFromLeftHand(uint32_t cardId, int discardPileSize) {
    _leftHand.removeCardId(cardId);
    takeCard(cardId);
    _discardPile.setCount(discardPileSize);
    reorganizeHand(_leftHand);
}

void BattleUI::discardFromRightHand(uint32_t cardId, int discardPileSize) {
    _rightHand.removeCardId(cardId);
    takeCard(cardId);
    _discardPile.setCount(discardPileSize);
    reorganizeHand(_rightHand);
}

#pragma endregion

#pragma region Helpers

UnitUI* BattleUI::findUnit(uint32_t unitId) {
    auto it = std::find_if(_units.begin(), _units.end(), [unitId](const UnitUI& unit) {
        return unit.id() == unitId;
    });
    if (it == _units.end()) {
        TraceLog(LOG_WARNING, "BattleUI: unit %u not found in unit list", unitId);
        return nullptr;
    }
    return &*it;
}

CardUI BattleUI::takeCard(uint32_t cardId) {
    auto it = std::find_if(_cards.begin(), _cards.end(), [cardId](const CardUI& card) {
        return card.id() == cardId;
    });
    if (it == _cards.end()) {
        TraceLog(LOG_ERROR, "BattleUI: takeCard %u not found in card list", cardId);
        return CardUI(cardId); // shouldn't happen; indicates a logic error upstream
    }

    CardUI card = *it; // transform is parentless here, so the copy is safe
    _cards.erase(it);
    return card;
}

void BattleUI::addCard(uint32_t cardId, std::string texture) {
    CardUI c(cardId);
    c.transform = _drawPile.getAnchorPoint().transform.asWorldTransform();
    c.sprite.texture = texture;
    c.hitbox.setRectangle(c.sprite.size);
    _cards.push_back(c);
}

void BattleUI::reorganizeHand(const FixedSlotHandUI& hand) {
    size_t slot = 0;
    for (uint32_t id : hand.getCardIds()) {
        if (slot >= hand.slotCount()) {
            TraceLog(
                LOG_WARNING,
                "BattleUI: hand holds %d cards but only %d slots; %d not placed",
                (int)hand.getCardIds().size(),
                (int)hand.slotCount(),
                (int)hand.getCardIds().size() - (int)hand.slotCount()
            );
            break;
        }

        auto it = std::find_if(_cards.begin(), _cards.end(), [id](const CardUI& card) {
            return card.id() == id;
        });
        if (it != _cards.end()) {
            const TransformComponent& target = hand.getPosition(slot).transform;
            it->transform.position = target.position;
            it->transform.rotation = target.rotation;
            it->transform.scale = target.scale;
        } else {
            TraceLog(
                LOG_WARNING, "BattleUI: card %u is in a hand but missing from the card list", id
            );
        }
        ++slot;
    }
}

#pragma endregion
