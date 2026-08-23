#include "UI/boardPieces/constructionUI.h"
#include <string>

ConstructionUI::ConstructionUI(
    uint32_t id,
    HexCoord coord,
    std::string splashArt,
    std::string name,
    int durability,
    const ConstructionUIConfig& config
)
    : BoardPieceUI(id, coord, std::move(splashArt), std::move(name), config.piece) {
    const BoardPieceUIConfig& base = config.piece;

    durabilityText.text.text = std::to_string(durability);
    durabilityText.text.fontSize = base.textFontSize;
    durabilityText.text.color = base.textColor;
    durabilityText.text.outlineColor = base.textOutlineColor;
    durabilityText.text.outlineThickness = base.textOutlineThickness;
    durabilityText.transform.position = config.durabilityTextOffset;

    durabilityPip.sprite.texture = config.durabilityPipTexture;
    durabilityPip.sprite.size = base.pipSize;
    durabilityPip.transform.position = config.durabilityPipOffset;

    transform.addChild(&durabilityText.transform);
    transform.addChild(&durabilityPip.transform);
}

void ConstructionUI::setDurability(int durability) {
    durabilityText.text.text = std::to_string(durability);
}
