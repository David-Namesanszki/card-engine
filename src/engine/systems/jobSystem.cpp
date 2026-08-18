#include "engine/systems/jobSystem.h"

JobSystem::JobSystem(const size_t threadCount) {
    _threads.reserve(threadCount);
    for (size_t i = 0; i < threadCount; i++) {
        _threads.emplace_back([this]() { workerLoop(); });
    }
}

JobSystem::~JobSystem() {
    terminate();

    for (auto& t : _threads) {
        t.join();
    }
}

void JobSystem::terminate() {
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _isTerminated = true;
    }
    _isJobsFull.notify_all();
    _isJobsEmpty.notify_all();
}

void JobSystem::addJob(std::function<void()> job) {
    {
        std::unique_lock<std::mutex> lock(_mtx);
        _isJobsFull.wait(lock, [this]() {
            return _jobs.size() < MAX_SIZE || _isTerminated == true;
        });
        if (_isTerminated == true)
            return;

        _jobs.push_back(std::move(job));
    }
    _isJobsEmpty.notify_one();
}

void JobSystem::workerLoop() {
    for (;;) {
        std::unique_lock<std::mutex> lock(_mtx);
        _isJobsEmpty.wait(lock, [this]() { return !_jobs.empty() || _isTerminated == true; });
        if (_isTerminated == true)
            return;

        auto j = std::move(_jobs.front());
        _jobs.pop_front();
        lock.unlock();

        _isJobsFull.notify_one();
        try {
            j();
        } catch (...) {
        }
    }
}