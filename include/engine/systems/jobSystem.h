#pragma once

#include <vector>
#include <functional>
#include <thread>
#include <atomic>
#include <deque>
#include <mutex>
#include <condition_variable>

class JobSystem {
  public:
    JobSystem(const size_t threadCount);
    ~JobSystem();
    void addJob(std::function<void()> job);
    void terminate();

  private:
    void workerLoop();

    const size_t MAX_SIZE = 1024;

    std::mutex _mtx;
    std::condition_variable _isJobsFull;
    std::condition_variable _isJobsEmpty;
    bool _isTerminated = false;
    std::deque<std::function<void()>> _jobs;
    std::vector<std::thread> _threads;
};