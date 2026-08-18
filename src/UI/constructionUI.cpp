#include "UI/constructionUI.h"
#include <string>

ConstructionUI::ConstructionUI(uint32_t id, const ConstructionUIConfig& config)
    : _id(id),
      _nameCenter(config.namePipOffset) {
    sprite.size = config.spriteSize;
    hitbox.setRectangle(config.spriteSize);

    for (TextUI* text : {&durabilityText, &nameText}) {
        text->text.text = "?";
        text->text.fontSize = config.textFontSize;
        text->text.color = config.textColor;
        text->text.outlineColor = config.textOutlineColor;
        text->text.outlineThickness = config.textOutlineThickness;
    }
    nameText.text.fontSize = config.nameFontSize;

    namePip.sprite.texture = config.namePipTexture;
    namePip.sprite.size = config.namePipSize;
    namePip.transform.position = config.namePipOffset;

    durabilityPip.sprite.texture = config.durabilityPipTexture;
    durabilityPip.sprite.size = config.pipSize;
    durabilityPip.transform.position = config.durabilityPipOffset;
    durabilityText.transform.position = config.durabilityTextOffset;

    transform.addChild(&durabilityText.transform);
    transform.addChild(&durabilityPip.transform);
    transform.addChild(&nameText.transform);
    transform.addChild(&namePip.transform);
}

void ConstructionUI::setDurability(int durability) {
    durabilityText.text.text = std::to_string(durability);
}

void ConstructionUI::setName(const std::string& name) {
    nameText.text.text = name;
    // DrawText anchors top-left, so centre the variable-length name on the
    // banner by measuring it.
    nameText.transform.position = {
        _nameCenter.x - MeasureText(name.c_str(), nameText.text.fontSize) / 2.0f,
        _nameCenter.y - nameText.text.fontSize / 2.0f
    };
}
