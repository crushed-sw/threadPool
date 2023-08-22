#ifndef __RESULT_HPP
#define __RESULT_HPP

#include "Any.hpp"
#include "Semaphore.hpp"
#include "Task.hpp"

#include <atomic>

class Result {
public:
    Result(std::shared_ptr<Task> task, bool is = true); 
    ~Result();

    void setValue(Any any);
    Any get();

private:
    Any any_;
    Semaphore sem_;
    std::shared_ptr<Task> task_;
    std::atomic_bool isValid_;
};

#endif //__RESULT_HPP
