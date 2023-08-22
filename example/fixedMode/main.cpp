#include "../../include/ThreadPool.hpp"

#include <iostream>
#include <chrono>

class Demo : public Task {
public:
    Any run() override {
        std::cout << "run in thread: " << std::this_thread::get_id() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return "";
    }

};

int main() {
    {
        ThreadPool pool;

        pool.setMode(PoolMode::MODE_FIXED);
        pool.start(2);

        std::cout << "Run 2 thread in threadpool with fixed mode:" << std::endl;

        pool.submitTask(std::make_shared<Demo>());
        pool.submitTask(std::make_shared<Demo>());
        pool.submitTask(std::make_shared<Demo>());
        pool.submitTask(std::make_shared<Demo>());
    }
}
