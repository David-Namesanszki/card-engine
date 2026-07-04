#include "engine/systems/targetSystem.h"
#include <algorithm>

void TargetSystem::reset() {
    _pickCount = 0;
    _selectedTargets.clear();
    _possibleTargets.clear();
}

bool TargetSystem::tryAddTarget(uint32_t target) {
    auto it = std::find(_possibleTargets.begin(), _possibleTargets.end(), target);

    if (it == _possibleTargets.end()) {
        return false;
    }
    _selectedTargets.push_back(target);
    return true;
}

std::vector<uint32_t> TargetSystem::getSelectedTargets() const {
    return _selectedTargets;
}

bool TargetSystem::isFilled() const {
    return _selectedTargets.size() >= _pickCount;
}

void TargetSystem::init(const std::vector<uint32_t>& possibleTargets, int pickCount) {
    _pickCount = pickCount;
    _possibleTargets = possibleTargets;
}