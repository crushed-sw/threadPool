#ifndef __SEMAPHORE_HPP
#define __SEMAPHORE_HPP

#include <mutex>
#include <atomic>
#include <condition_variable>

class Semaphore {
public:
    Semaphore(int limit = 0);
    ~Semaphore();

    void wait();
    void post();

private:
    std::atomic_bool exit_;
    int resLimit_;
    std::mutex mutex_;
    std::condition_variable cond_;
};

#endif //__SEMAPHORE_HPP
