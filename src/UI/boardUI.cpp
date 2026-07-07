#include "UI/boardUI.h"
#include <algorithm>

namespace {
// Placeholder hex radius in pixels until board layout becomes configurable.
constexpr float kHexSize = 50.0f;
} // namespace

BoardUI::BoardUI(const BoardUIConfig& config) {
    _tiles.reserve(config.tileDatas.size());

    uint32_t nextId = 0;
    for (const auto& data : config.tileDatas) {
        _tiles.emplace_back(nextId++, data.type(), data.team());
        Vector2 hexPos = hexToPixelFlat(data.coord(), kHexSize);
        _tiles.back().transform.position = {
            config.transform.position.x + hexPos.x, config.transform.position.y + hexPos.y
        };
    }

    std::stable_sort(_tiles.begin(), _tiles.end(), [](const BoardTileUI& a, const BoardTileUI& b) {
        return a.transform.position.y < b.transform.position.y;
    });

    transform = config.transform;
    sprite = config.sprite;
}
