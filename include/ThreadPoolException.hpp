#ifndef __THREAD_POOL_EXCEPTIONA
#define __THREAD_POOL_EXCEPTIONA

#include <stdexcept>
#include <string>

class ThreadPoolException : public std::exception {
public:
    ThreadPoolException(const std::string& str);
    ~ThreadPoolException();

    const char* what() const noexcept override;

private:
    std::string str_;
};

#endif //__THREAD_POOL_EXCEPTIONA
