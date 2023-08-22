#include "../include/Semaphore.hpp"

Semaphore::Semaphore(int limit) : resLimit_(limit), exit_(false) {}

Semaphore::~Semaphore() {
    exit_ = true;
}

void Semaphore::wait() {
    if(exit_)
        return;
    std::unique_lock<std::mutex> locker(mutex_);
    cond_.wait(locker, [&]()->bool { return resLimit_ > 0; });
    resLimit_--;
}

void Semaphore::post() {
    if(exit_)
        return;
    std::unique_lock<std::mutex> locker(mutex_);
    resLimit_++;
    cond_.notify_all();
}
