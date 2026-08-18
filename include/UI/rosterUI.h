#pragma once

#include <list>
#include <string>
#include <cstdint>
#include "UI/scrollViewUI.h"
#include "UI/scrollableGridUI.h"
#include "UI/unitUI.h"
#include "UI/constructionUI.h"
#include "UI/configs/rosterUIConfig.h"

class RosterUI {
  public:
    RosterUI(RosterUIConfig config = {});

    void addUnit(uint32_t id);
    void removeUnit(uint32_t id);
    void addConstruction(uint32_t id);
    void removeConstruction(uint32_t id);

    const std::list<uint32_t>& units() const {
        return _units;
    }
    const std::list<uint32_t>& constructions() const {
        return _constructions;
    }

    TransformComponent transform;
    SpriteComponent sprite;
    HitBoxComponent hitbox;

  private:
    RosterUIConfig _config;
    std::list<uint32_t> _units;
    std::list<uint32_t> _constructions;
};
