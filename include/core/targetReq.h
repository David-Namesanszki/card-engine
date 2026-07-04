#pragma once

#include <vector>

struct TargetReq {
    int neededTargets;
    std::vector<uint32_t> possibleTargets;
};