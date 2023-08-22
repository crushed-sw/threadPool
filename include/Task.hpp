#ifndef __TASK_HPP
#define __TASK_HPP

#include "Any.hpp"

class Result;

class Task {
public:
    Task(Result* result = nullptr);
    ~Task();

    virtual Any run() = 0;
    void execute();
    void setResult(Result* result);

private:
    Result* result_;
};

#endif //__TASK_HPP
