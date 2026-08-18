#pragma once

#include <cstdint>
#include "core/captain.h"

class Raid {
  public:
    Raid(
        const std::string& captainName,
        const Health& captainHealth,
        size_t rosterSize,
        int fireCount,
        int actionPointCount
    )
        : _captain(captainName, captainHealth, rosterSize, actionPointCount, fireCount) {};

  private:
    Captain _captain;
};