#include "../include/Result.hpp"

Result::Result(std::shared_ptr<Task> task, bool is) : task_(task), isValid_(is) {
    task_->setResult(this);
}

Result::~Result() {}

void Result::setValue(Any any) {
    any_ = std::move(any);
    sem_.post();
}

Any Result::get() {
    if(!isValid_) {
        return "";
    }

    sem_.wait();
    return std::move(any_);
}
