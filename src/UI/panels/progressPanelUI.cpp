#include "UI/panels/progressPanelUI.h"
#include <string>

ProgressPanelUI::ProgressPanelUI(ProgressPanelUIConfig config)
    : transform(config.transform),
      background(config.background) {
}

void ProgressPanelUI::setCurrentDays(int currentDays) {
    currentDaysText.text.text = std::to_string(currentDays);
}

void ProgressPanelUI::setMaxDays(int maxDays) {
    maxDaysText.text.text = std::to_string(maxDays);
}

void ProgressPanelUI::setRaidSection(int raidSection) {
    raidSectionCountText.text.text = std::to_string(raidSection);
}

void ProgressPanelUI::setBarkOre(int barkOre) {
    barkOreCountText.text.text = std::to_string(barkOre);
}

void ProgressPanelUI::setBloodSap(int bloodSap) {
    bloodSapCountText.text.text = std::to_string(bloodSap);
}

void ProgressPanelUI::setHeartWood(int heartWood) {
    heartWoodCountText.text.text = std::to_string(heartWood);
}
