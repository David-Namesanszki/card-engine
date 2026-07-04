#include "UI/panels/actionPointsPanelUI.h"
#include <algorithm>

ActionPointsPanelUI::ActionPointsPanelUI(ActionPointsPanelUIConfig config)
    : _config(config) {
    transform = config.transform;
    sprite = config.background;

    _pips.resize(_config.maxActionPoints);
    for (int i = 0; i < _config.maxActionPoints; i++) {
        Pip pip;
        pip.transform.position = slotPosition(i);
        pip.sprite.texture = _config.availableTexture;
        pip.sprite.size = _config.pipSize;
        _pips[i] = pip;
    }
}

void ActionPointsPanelUI::setSpent(int spentCount) {
    spentCount = std::clamp(spentCount, 0, _config.maxActionPoints);
    for (int i = 0; i < (int)_pips.size(); i++)
        _pips[i].sprite.texture =
            (i < spentCount) ? _config.spentTexture : _config.availableTexture;
}

Vector2 ActionPointsPanelUI::slotPosition(int slot) const {
    float t = (_config.maxActionPoints <= 1)
                  ? 0.5f
                  : static_cast<float>(slot) / static_cast<float>(_config.maxActionPoints - 1);
    Vector2 start = {
        transform.position.x + _config.lineStartOffset.x,
        transform.position.y + _config.lineStartOffset.y
    };
    Vector2 end = {
        transform.position.x + _config.lineEndOffset.x,
        transform.position.y + _config.lineEndOffset.y
    };
    return {start.x + (end.x - start.x) * t, start.y + (end.y - start.y) * t};
}
