#include "UI/panels/actionPointsPanelUI.h"
#include <algorithm>

ActionPointsPanelUI::ActionPointsPanelUI(ActionPointsPanelUIConfig config)
    : transform(config.transform),
      sprite(config.background),
      _config(config) {
}

// Rebuilds the pip row. Only the maximum decides how many pips exist and where
// they sit, so this is the only place that touches the vector.
void ActionPointsPanelUI::setMaxActionPoints(int max) {
    max = std::max(max, 0);
    if (max == _maxActionPoints)
        return;

    _maxActionPoints = max;
    _currentActionPoints = std::clamp(_currentActionPoints, 0, _maxActionPoints);

    // slotPosition() spaces the row using _maxActionPoints, so the new maximum
    // has to be in place before any position is computed.
    _pips.assign(_maxActionPoints, Pip{});
    for (int i = 0; i < _maxActionPoints; i++) {
        _pips[i].transform.position = slotPosition(i);
        _pips[i].sprite.size = _config.pipSize;
    }

    applyPipTextures();
}

// Spending never changes how many pips there are — only which of them are lit.
void ActionPointsPanelUI::setCurrentActionPoints(int current) {
    _currentActionPoints = std::clamp(current, 0, _maxActionPoints);
    applyPipTextures();
}

// Pips deplete right to left: the leftmost _currentActionPoints stay available,
// the rest read as spent. Flip the comparison to deplete the other way.
void ActionPointsPanelUI::applyPipTextures() {
    for (int i = 0; i < static_cast<int>(_pips.size()); i++)
        _pips[i].sprite.texture =
            (i < _currentActionPoints) ? _config.availableTexture : _config.spentTexture;
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
