#include "UI/panels/actionPointsPanelUI.h"
#include <algorithm>

ActionPointsPanelUI::ActionPointsPanelUI(ActionPointsPanelUIConfig config)
    : transform(config.transform),
      sprite(config.background),
      _config(config) {
}

void ActionPointsPanelUI::setActionPoints(int current, int max) {
    if (max != _maxActionPoints) {
        _maxActionPoints = max;
        _pips.assign(max, Pip{});
        for (int i = 0; i < max; i++) {
            _pips[i].transform.position = slotPosition(i);
            _pips[i].sprite.texture = _config.availableTexture;
            _pips[i].sprite.size = _config.pipSize;
        }
    }

    int spentCount = std::clamp(max - current, 0, _maxActionPoints);
    for (int i = 0; i < (int)_pips.size(); i++)
        _pips[i].sprite.texture =
            (i < spentCount) ? _config.spentTexture : _config.availableTexture;
}

Vector2 ActionPointsPanelUI::slotPosition(int slot) const {
    float t = (_maxActionPoints <= 1)
                  ? 0.5f
                  : static_cast<float>(slot) / static_cast<float>(_maxActionPoints - 1);
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
