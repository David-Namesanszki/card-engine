#pragma once

#include "renderObject.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>

struct BattleLayout {
    Vector2     drawPilePos    = {60,   305};
    std::string drawPileImage;
    Vector2     discardPilePos = {1140, 305};
    std::string discardPileImage;
    Vector2     leftHandPos    = {220,  305};
    std::string leftHandImage;
    Vector2     rightHandPos   = {220,  155};
    std::string rightHandImage;
    float       cardWidth      = 80.0f;
    float       cardHeight     = 110.0f;
    float       cardSpacing    = 95.0f;
    int         handSize       = 5;
};

class Renderer {
public:
    Renderer();
    uint32_t      create(RenderObject obj = {});
    void          destroy(uint32_t id);
    RenderObject& get(uint32_t id);
    void          draw(const std::vector<const RenderObject*>& extras = {});
    const BattleLayout& getLayout() const { return _layout; }
private:
    void drawObject(const RenderObject& obj);
    std::vector<const RenderObject*> sorted(const std::vector<const RenderObject*>& extras) const;
    BattleLayout _layout;
    uint32_t     _nextId = 1;
    std::unordered_map<uint32_t, RenderObject> _objects;
};
