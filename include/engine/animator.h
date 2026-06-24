#pragma once

#include <functional>
#include <vector>
#include <cstdint>

using EasingFn = std::function<float(float)>;

struct Channel {
    float                      from;
    float                      to;
    std::function<void(float)> setter;
};

struct Animation {
    std::vector<Channel>  channels;
    float                 duration   = 0.0f;
    float                 delay      = 0.0f;
    float                 elapsed    = 0.0f;
    uint32_t              tag        = 0;
    EasingFn              easing;
    std::function<void()> onComplete = nullptr;
};

class Animator {
public:
    void add(Animation anim);
    void cancel(uint32_t tag);
    void update(float dt);
    int  activeCount() const { return static_cast<int>(_animations.size()); }
private:
    std::vector<Animation> _animations;
};
