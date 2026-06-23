#include "UI/battleUI.h"

BattleUI::BattleUI(
    const BattleUIConfig& config,
    const LeftHandUIConfig& leftHandConfig,
    const RightHandUIConfig& rightHandConfig,
    const DiscardPileUIConfig& discardPileConfig,
    const DrawPileUIConfig& drawPileConfig) {
}

void BattleUI::drawCard(uint32_t cardId) {
    CardUI c(cardId);
    const AnchorPoint& drawPoint = _drawPile.getDrawPoint();

    c.position = drawPoint.position;
    c.rotation = drawPoint.rotation;
    c.scale = drawPoint.scale;

    _leftHand.addCard(c);

    if (_config.drawAnimDef) {
        _animator.add(_config.drawAnimDef(c, _leftHand.getSlot0()));
    }
}

void BattleUI::discardFromLeftHand(uint32_t cardId) {
    CardUI card =_leftHand.removeCard(cardId);

    if (_config.discardAnimDef) {
        _animator.add(_config.discardAnimDef(card, _discardPile.getDiscardPoint()));
    }
}

void BattleUI::discardFromRightHand(uint32_t cardId) {
    CardUI card =_rightHand.removeCard(cardId);

    if (_config.discardAnimDef) {
        _animator.add(_config.discardAnimDef(card, _discardPile.getDiscardPoint()));
    }
}
