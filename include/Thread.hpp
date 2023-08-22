#ifndef __THREAD_HPP
#define __THREAD_HPP

#include <functional>
#include <thread>

class Thread {
public:
    using TaskFunc = std::function<void(int)>;

    Thread(TaskFunc func);
    ~Thread();

    void start();
    int getId();

private:
    TaskFunc func_;
    int threadId_;

    static int id_;
};

#endif //__THREAD_HPP
