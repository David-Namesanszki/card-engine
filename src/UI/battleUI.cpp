#include "UI/battleUI.h"
#include "raylib.h"
#include "engine/easing.h"
#include <algorithm>
#include <random>
#include <vector>

BattleUI::BattleUI(
    Animator& animator,
    BattleUIConfig config,
    LeftHandUIConfig leftHandConfig,
    RightHandUIConfig rightHandConfig,
    DiscardPileUIConfig discardPileConfig,
    DrawPileUIConfig drawPileConfig)
    : _animator(animator)
    , _config(config)
    , _drawPile(drawPileConfig)
    , _discardPile(animator, discardPileConfig)
    , _leftHand(animator, leftHandConfig)
    , _rightHand(animator, rightHandConfig) {
}

void BattleUI::drawCard() {
    if (_drawPile.empty())
        return;
    _leftHand.addCard(_drawPile.removeTopCard());
}

void BattleUI::discardFromLeftHand(uint32_t cardId) {
    _discardPile.add(_leftHand.removeCard(cardId));
}

void BattleUI::discardFromRightHand(uint32_t cardId) {
    _discardPile.add(_rightHand.removeCard(cardId));
}

void BattleUI::reshuffleDiscardIntoDraw() {
    if (_discardPile.empty())
        return;

    Vector2 from = _discardPile.transform.position;
    Vector2 to   = _drawPile.transform.position;

    // Logical move is instant; the sweep below is pure presentation.
    std::vector<CardUI> cards;
    while (!_discardPile.empty())
        cards.push_back(_discardPile.removeTopCard());

    static std::mt19937 rng{std::random_device{}()};
    std::shuffle(cards.begin(), cards.end(), rng);

    for (auto& card : cards)
        _drawPile.add(std::move(card));

    playReshuffleEffect(from, to);
}

void BattleUI::playReshuffleEffect(Vector2 from, Vector2 to) {
    constexpr int   ghostCount = 3;
    constexpr float duration   = 0.35f;
    constexpr float stagger    = 0.08f;

    for (int i = 0; i < ghostCount; ++i) {
        auto it = _reshuffleGhosts.insert(_reshuffleGhosts.end(), TransformComponent{});
        it->position = from;

        Animation anim;
        anim.duration = duration;
        anim.delay    = stagger * i;
        anim.easing   = Easing::easeInOut;
        anim.channels = {
            {from.x, to.x, [it](float v) { it->position.x = v; }},
            {from.y, to.y, [it](float v) { it->position.y = v; }},
        };
        anim.onComplete = [this, it]() { _reshuffleGhosts.erase(it); };
        _animator.add(std::move(anim));
    }
}

void BattleUI::draw() const {
    static const Color COLOR_PILE   = {60,  60,  60,  255};
    static const Color COLOR_DISCARD = {80,  30,  30,  255};
    static const Color COLOR_CARD   = {70,  100, 140, 255};
    static const float W = 60.0f, H = 90.0f;

    Vector2 dp = _drawPile.transform.position;
    DrawRectangle((int)dp.x - 30, (int)dp.y - 45, (int)W, (int)H, COLOR_PILE);

    Vector2 disc = _discardPile.transform.position;
    DrawRectangle((int)disc.x - 30, (int)disc.y - 45, (int)W, (int)H, COLOR_DISCARD);

    for (const auto& card : _leftHand.cards()) {
        Vector2 pos = card.transform.position;
        DrawRectanglePro({pos.x, pos.y, W * card.transform.scale, H * card.transform.scale},
                         {W / 2 * card.transform.scale, H / 2 * card.transform.scale},
                         card.transform.rotation, COLOR_CARD);
    }

    for (const auto& card : _rightHand.cards()) {
        Vector2 pos = card.transform.position;
        DrawRectanglePro({pos.x, pos.y, W * card.transform.scale, H * card.transform.scale},
                         {W / 2 * card.transform.scale, H / 2 * card.transform.scale},
                         card.transform.rotation, {140, 70, 100, 255});
    }

    for (const auto& ghost : _reshuffleGhosts) {
        Vector2 pos = ghost.position;
        DrawRectanglePro({pos.x, pos.y, W, H}, {W / 2, H / 2}, ghost.rotation, {120, 120, 160, 180});
    }
}
