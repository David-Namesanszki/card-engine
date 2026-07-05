#include "UI/panels/progressPanelUI.h"
#include <string>

ProgressPanelUI::ProgressPanelUI(ProgressPanelUIConfig config)
    : transform(config.transform),
      background(config.background) {
    // Five stat rows stacked down the 128x216 panel, each a pip icon on the
    // left and its value beside it; the days row shows current/max side by
    // side. Placeholder values until the core exposes real progress data.
    transform.addChild(&daysPip.transform);
    transform.addChild(&raidSectionPip.transform);
    transform.addChild(&barkOrePip.transform);
    transform.addChild(&bloodSapPip.transform);
    transform.addChild(&heartWoodPip.transform);
    transform.addChild(&currentDaysText.transform);
    transform.addChild(&maxDaysText.transform);
    transform.addChild(&raidSectionCountText.transform);
    transform.addChild(&barkOreCountText.transform);
    transform.addChild(&bloodSapCountText.transform);
    transform.addChild(&heartWoodCountText.transform);

    daysPip.transform.position = {-44.0f, -80.0f};
    daysPip.sprite.texture = config.daysPipTexture;
    daysPip.sprite.size = config.pipSize;
    currentDaysText.transform.position = {-25.0f, -86.0f};
    currentDaysText.text.text = "1";
    maxDaysText.transform.position = {5.0f, -86.0f};
    maxDaysText.text.text = "30";

    raidSectionPip.transform.position = {-44.0f, -40.0f};
    raidSectionPip.sprite.texture = config.raidSectionPipTexture;
    raidSectionPip.sprite.size = config.pipSize;
    raidSectionCountText.transform.position = {-10.0f, -46.0f};
    raidSectionCountText.text.text = "1";

    barkOrePip.transform.position = {-44.0f, 0.0f};
    barkOrePip.sprite.texture = config.barkOrePipTexture;
    barkOrePip.sprite.size = config.pipSize;
    barkOreCountText.transform.position = {-10.0f, -6.0f};
    barkOreCountText.text.text = "0";

    bloodSapPip.transform.position = {-44.0f, 40.0f};
    bloodSapPip.sprite.texture = config.bloodSapPipTexture;
    bloodSapPip.sprite.size = config.pipSize;
    bloodSapCountText.transform.position = {-10.0f, 34.0f};
    bloodSapCountText.text.text = "0";

    heartWoodPip.transform.position = {-44.0f, 80.0f};
    heartWoodPip.sprite.texture = config.heartWoodPipTexture;
    heartWoodPip.sprite.size = config.pipSize;
    heartWoodCountText.transform.position = {-10.0f, 74.0f};
    heartWoodCountText.text.text = "0";
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
