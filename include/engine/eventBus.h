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
};