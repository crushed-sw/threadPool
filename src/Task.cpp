#include "../include/Task.hpp"
#include "../include/Result.hpp"

Task::Task(Result* result) : result_(result) {}

Task::~Task() {}

void Task::execute() {
    if(result_ != nullptr)
        result_->setValue(run());
}

void Task::setResult(Result* result) {
    result_ = result;
}
