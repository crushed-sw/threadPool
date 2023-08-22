#ifndef __ANY_HPP
#define __ANY_HPP

#include "AnyWrap.hpp"

#include <memory>
#include <stdexcept>

class Any {
public:
    Any() = default;
    ~Any() = default;
    Any(const Any&) = delete;
    Any& operator=(const Any&) = delete;
    Any(Any&&) = default;
    Any& operator=(Any&&) = default;

    template <typename T>
    Any(T data);

    template <typename T>
    T cast();

private:
    std::unique_ptr<AnyBase> base_;
};

template <typename T>
Any::Any(T data) : base_(std::make_unique<AnyDerive<T>>(data)) {}

template <typename T>
T Any::cast() {
    AnyDerive<T>* temp = dynamic_cast<AnyDerive<T>*>(base_.get());
    if(temp == nullptr)
        throw std::runtime_error("type is unmach");
    return temp->data_;
}

#endif //__ANY_HPP
