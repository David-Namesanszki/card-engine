#pragma once

#include <vector>
#include <cstdint>

class TargetSystem {
  public:
    void reset();
    bool tryAddTarget(uint32_t target);
    std::vector<uint32_t> getSelectedTargets() const;
    bool isFilled() const;
    void init(const std::vector<uint32_t>& possibleTargets, int pickCount);

    const std::vector<uint32_t>& getPossibleTargets() const {
        return _possibleTargets;
    }

  private:
    int _pickCount = 0;
    std::vector<uint32_t> _selectedTargets;
    std::vector<uint32_t> _possibleTargets;
};