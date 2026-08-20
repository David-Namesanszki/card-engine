#pragma once

#include <cstdint>

// Base for everything the game refers to by id: board pieces, and anything
// else that needs a stable handle. Ids are assigned on construction and are
// unique for the lifetime of the process; 0 is never issued, so it is free to
// use as an "no entity" sentinel.
class Entity {
  public:
    Entity()
        : _id(_nextId++) {
    }

    uint32_t getId() const {
        return _id;
    }

    virtual ~Entity() = default;

  private:
    // `inline` so every translation unit shares one counter -- a plain
    // `static` at namespace scope would give each .cpp its own, and ids
    // would collide across files.
    static inline uint32_t _nextId = 1;

    uint32_t _id;
};
