#ifndef __THREAD_POOL_HPP
#define __THREAD_POOL_HPP

#include "Result.hpp"
#include "Task.hpp"
#include "Thread.hpp"
#include "ThreadPoolException.hpp"

#include <chrono>
#include <unordered_map>
#include <queue>
#include <memory>
#include <atomic>
#include <mutex>
#include <thread>
#include <functional>
#include <condition_variable>

const int TASK_DEFAULT_MAX_SIZE = INT32_MAX;
const int THREAD_DEFAULT_MAX_SIZE = 32;
const int THREAD_DEFAULT_SIZE = std::thread::hardware_concurrency();
const int THREAD_MAX_IDEL_TIME = 60;

/**
 * 线程池模式（fixed cached）
 */
enum class PoolMode {
    MODE_FIXED,
    MODE_CACHED
};

/**
 * 线程池类
 */
class ThreadPool {
public:
    ThreadPool();
    ~ThreadPool();

    void setMode(PoolMode mode);
    void setTaskMaxSize(size_t size);
    void setThreadMaxSize(size_t size);
    Result submitTask(std::shared_ptr<Task> task);
    void start(size_t initSize);

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

private:
    void threadHandler(int threadId);
    bool checkRunning() const;

private:
    std::unordered_map<int, std::unique_ptr<Thread>> threads_;
    size_t initThreadSize_;
    size_t threadMaxSize_;
    std::atomic_uint threadSize_;
    std::atomic_uint idleThreadSize_;

    std::queue<std::shared_ptr<Task>> taskQueue_;
    std::atomic_uint taskSize_;
    size_t taskQueueMaxSize_;

    std::mutex mutex_;
    std::condition_variable notFull_;
    std::condition_variable notEmpty_;
    std::condition_variable exitCond_;

    PoolMode poolMode_;

    std::atomic_bool isRunning_;
};

#endif //__THREAD_POOL_HPP
