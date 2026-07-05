#include "UI/captainUI.h"
#include <string>

CaptainUI::CaptainUI(CaptainUIConfig config)
    : transform(config.transform),
      sprite(config.sprite) {
    transform.addChild(&healthPip.transform);
    transform.addChild(&healthText.transform);

    // Health readout below the portrait: pip icon with the value beside it.
    healthPip.transform.position = {-25.0f, 76.0f};
    healthPip.sprite.texture = config.healthPipTexture;
    healthPip.sprite.size = config.pipSize;

    healthText.transform.position = {-5.0f, 70.0f};
    healthText.text.text = "";
}

void CaptainUI::setHealth(int current, int max) {
    healthText.text.text = std::to_string(current) + "/" + std::to_string(max);
}
