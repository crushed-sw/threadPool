#include "../include/ThreadPoolException.hpp"

ThreadPoolException::ThreadPoolException(const std::string& str) : str_(str) {}

ThreadPoolException::~ThreadPoolException() {}

const char* ThreadPoolException::what() const noexcept {
    return str_.c_str();
}
