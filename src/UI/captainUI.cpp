#include "UI/captainUI.h"
#include <string>

void CaptainUI::setHealth(int health) {
    healthText.text.text = std::to_string(health);
}
