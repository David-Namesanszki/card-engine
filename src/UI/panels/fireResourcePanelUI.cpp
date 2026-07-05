#include "UI/panels/fireResourcePanelUI.h"
#include <string>

FireResourcePanelUI::FireResourcePanelUI(FireResourcePanelUIConfig config)
    : transform(config.transform),
      background(config.background) {
    transform.addChild(&firePip.transform);
    transform.addChild(&fireCountText.transform);

    firePip.transform.position = {-25.0f, 0.0f};
    firePip.sprite.texture = config.firePipTexture;
    firePip.sprite.size = config.pipSize;

    fireCountText.transform.position = {-5.0f, -6.0f};
    fireCountText.text.text = "0";
}

void FireResourcePanelUI::setFireCount(int fireCount) {
    fireCountText.text.text = std::to_string(fireCount);
}
