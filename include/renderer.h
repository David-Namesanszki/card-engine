#pragma once

#include "raylib.h"
#include <string>

class Battle;

struct BattleLayout {
    Vector2     drawPilePos       = {60,   305};
    std::string drawPileImage;
    Vector2     discardPilePos    = {1140, 305};
    std::string discardPileImage;
    Vector2     leftHandPos       = {220,  305};
    std::string leftHandImage;
    Vector2     rightHandPos      = {220,  155};
    std::string rightHandImage;
    float       cardWidth         = 80.0f;
    float       cardHeight        = 110.0f;
    float       cardSpacing       = 95.0f;
    int         handSize          = 5;
};

class Renderer {
public:
    Renderer();
    void drawBattle(const Battle& battle);
private:
    void drawCardSlot(Vector2 pos, Color fill, const char* label = "");
    void drawPileSlot(Vector2 pos, Color fill, int count);
    BattleLayout _layout;
};
