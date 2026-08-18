#pragma once

#include <cstdint>
#include <string>
#include "engine/components/transformComponent.h"
#include "engine/components/spriteComponent.h"
#include "engine/components/hitBoxComponent.h"
#include "UI/textUI.h"
#include "UI/panels/pip.h"
#include "UI/configs/constructionUIConfig.h"

class ConstructionUI {
  public:
    ConstructionUI(uint32_t id, const ConstructionUIConfig& config = {});

    ConstructionUI(const ConstructionUI&) = delete;
    ConstructionUI& operator=(const ConstructionUI&) = delete;

    uint32_t id() const {
        return _id;
    }

    void setDurability(int durability);
    void setName(const std::string& name);

    TransformComponent transform;
    SpriteComponent sprite;
    HitBoxComponent hitbox;

    TextUI durabilityText;
    Pip durabilityPip;

    TextUI nameText;
    Pip namePip;

  private:
    uint32_t _id;
    Vector2 _nameCenter;
};
