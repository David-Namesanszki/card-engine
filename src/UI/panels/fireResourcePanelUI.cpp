#include "UI/panels/fireResourcePanelUI.h"
#include <string>

FireResourcePanelUI::FireResourcePanelUI(FireResourcePanelUIConfig config)
    : transform(config.transform),
      background(config.background) {
}

void FireResourcePanelUI::setFireCount(int fireCount) {
    fireCountText.text.text = std::to_string(fireCount);
}
