#ifndef __ANY_WARP_HPP
#define __ANY_WARP_HPP

class Any;

class AnyBase {
public:
    virtual ~AnyBase() = default;
};

template <typename T>
class AnyDerive : public AnyBase {
public:
    AnyDerive(T data);
    ~AnyDerive();

private:
    T data_;

    friend Any;
};

template <typename T>
AnyDerive<T>::AnyDerive(T data) : data_(data) {}

template <typename T>
AnyDerive<T>::~AnyDerive() {}

#endif //__ANY_WARP_HPP
