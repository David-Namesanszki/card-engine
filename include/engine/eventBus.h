#pragma once
#include <vector>
#include <functional>

template<typename TEvent>
class EventBus {
    std::vector<std::function<void(TEvent)>> listeners;
public:
    void subscribe(std::function<void(TEvent)> callback) {
        listeners.push_back(callback);
    }

    void emit(TEvent event) {
        for (auto& listener : listeners)
            listener(event);
    }

    // Drops every listener. Owners expose this so subscribers that are about
    // to be destroyed (a scene being switched away) don't leave dangling
    // callbacks behind.
    void clear() {
        listeners.clear();
    }
};