#include "UI/buttonUI.h"

ButtonUI::ButtonUI(ButtonUIConfig config) : transform(config.transform), sprite(config.sprite) {
    hitbox.setRectangle(sprite.size);
}
