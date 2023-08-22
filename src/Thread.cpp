#include "../include/Thread.hpp"

int Thread::id_ = 0;

Thread::Thread(TaskFunc func) : func_(func), threadId_(id_++) {}

Thread::~Thread() {}

int Thread::getId() {
    return threadId_;
}

void Thread::start() {
    std::thread t(func_, threadId_);
    t.detach();
}
