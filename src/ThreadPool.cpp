#include "../include/ThreadPool.hpp"

#include <iostream>

ThreadPool::ThreadPool() 
    : initThreadSize_(THREAD_DEFAULT_SIZE)
    , threadMaxSize_(THREAD_DEFAULT_MAX_SIZE)
    , threadSize_(0)
    , idleThreadSize_(0)
    , taskSize_(0)
    , taskQueueMaxSize_(TASK_DEFAULT_MAX_SIZE)
    , poolMode_(PoolMode::MODE_FIXED)
    , isRunning_(false) {}

ThreadPool::~ThreadPool() {
    isRunning_ = false;
    std::unique_lock<std::mutex> locker(mutex_);
    notEmpty_.notify_all();
    exitCond_.wait(locker, [&]()->bool { return threads_.size() == 0; });
}

void ThreadPool::setMode(PoolMode mode) {
    if(checkRunning())
        return;
    poolMode_ = mode;
}

void ThreadPool::setTaskMaxSize(size_t size) {
    if(checkRunning())
        return;
    taskQueueMaxSize_ = size;
}

void ThreadPool::setThreadMaxSize(size_t size) {
    if(checkRunning() && poolMode_ == PoolMode::MODE_CACHED)
        return;
    threadMaxSize_ = size;
}

Result ThreadPool::submitTask(std::shared_ptr<Task> task) {
    std::unique_lock<std::mutex> locker(mutex_);
    if(!notFull_.wait_for(locker, std::chrono::seconds(1), [&]()->bool { return taskQueue_.size() < taskQueueMaxSize_; })) {
        return Result(task, false);
    }

    taskQueue_.emplace(task);
    taskSize_++;
    notEmpty_.notify_all();

    if(poolMode_ == PoolMode::MODE_CACHED && taskSize_ > idleThreadSize_ && threadSize_ < threadMaxSize_) {
        auto thread = std::make_unique<Thread>(std::bind(&ThreadPool::threadHandler, this, std::placeholders::_1));
        int id = thread->getId();
        threads_.emplace(id, std::move(thread));
        threads_[id]->start();
        threadSize_++;
        idleThreadSize_++;
    }
        
    return Result(task);
}

void ThreadPool::start(size_t initSize) {
    isRunning_ = true;
    initThreadSize_ = initSize;
    threadSize_ = initSize;

    for(unsigned int i = 0; i < initThreadSize_; ++i) {
        auto ptr = std::make_unique<Thread>(std::bind(&ThreadPool::threadHandler, this, std::placeholders::_1));
        int id = ptr->getId();
        threads_.emplace(id, std::move(ptr));
    }

    for(auto& thread : threads_) {
        thread.second->start();
        idleThreadSize_++;
    }
}

void ThreadPool::threadHandler(int threadId) {
    auto last = std::chrono::high_resolution_clock().now();
    while(true) {
        std::shared_ptr<Task> task;
        {
            std::unique_lock<std::mutex> locker(mutex_);
            
            if(poolMode_ == PoolMode::MODE_CACHED) {
                while(taskQueue_.size() == 0) {
                    if(!isRunning_) {
                        threads_.erase(threadId);
                        exitCond_.notify_all();
                        return;
                    }

                    if(std::cv_status::timeout == notEmpty_.wait_for(locker, std::chrono::seconds(1))) {
                        auto now = std::chrono::high_resolution_clock().now();
                        auto dur = std::chrono::duration_cast<std::chrono::seconds>(now - last);
                        if(dur.count() >= THREAD_MAX_IDEL_TIME && threadSize_ > initThreadSize_) {
                            threads_.erase(threadId);
                            threadSize_--;
                            idleThreadSize_--;
                            return;
                        }
                    } else {
                        notEmpty_.wait(locker);
                    }
                }
            }

            idleThreadSize_--;
            task = taskQueue_.front();
            taskQueue_.pop();
            taskSize_--;
            if(taskQueue_.size() != 0)
                notEmpty_.notify_all();
            notFull_.notify_all();
        }

        if(task != nullptr)
            task->execute();

        idleThreadSize_++;
        last = std::chrono::high_resolution_clock().now();
    }
}

bool ThreadPool::checkRunning() const {
    return isRunning_;
}
