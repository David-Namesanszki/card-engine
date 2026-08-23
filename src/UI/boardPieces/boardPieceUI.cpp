#include "UI/boardPieces/boardPieceUI.h"
#include "raylib.h"
#include <utility>

BoardPieceUI::BoardPieceUI(
    uint32_t id,
    HexCoord coord,
    std::string splashArt,
    std::string name,
    const BoardPieceUIConfig& config
)
    : EntityUI(id),
      _coord(coord),
      _nameCenter(config.namePipOffset) {
    sprite.texture = std::move(splashArt);
    sprite.size = config.spriteSize;
    hitbox.setRectangle(config.spriteSize);

    nameText.text.fontSize = config.nameFontSize;
    nameText.text.color = config.textColor;
    nameText.text.outlineColor = config.textOutlineColor;
    nameText.text.outlineThickness = config.textOutlineThickness;

    namePip.sprite.texture = config.namePipTexture;
    namePip.sprite.size = config.namePipSize;
    namePip.transform.position = config.namePipOffset;

    transform.addChild(&nameText.transform);
    transform.addChild(&namePip.transform);

    setName(name); // also centres it on the banner
}

void BoardPieceUI::setName(const std::string& name) {
    nameText.text.text = name;
    // DrawText anchors top-left, so centre the variable-length name on the
    // banner by measuring it.
    nameText.transform.position = {
        _nameCenter.x - MeasureText(name.c_str(), nameText.text.fontSize) / 2.0f,
        _nameCenter.y - nameText.text.fontSize / 2.0f
    };
}
