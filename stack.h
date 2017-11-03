
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <utility>
#include <mutex>
#include <memory>
#include <new>

template <typename T>
class stack
{
private:
    void swap(stack<T> &) noexcept;

    mutable std::mutex mtx;
public:
    stack() noexcept;
    stack(stack<T> const &) /* strong */;
    stack<T>& operator = (stack<T> const &)  noexcept;
    size_t count() const noexcept;
    void push(T const &) /* strong */;
    auto pop() -> std::shared_ptr<T>/*strong*/;


private:
    T* array_;
    size_t array_size_;
    size_t count_;
};

template <typename T>
stack<T>::stack() noexcept : count_(0), array_size_(0), array_(nullptr)
{}

template <typename T>
size_t stack<T>::count() const noexcept
{
    std::lock_guard<std::mutex> a(mtx);
    return count_;

}

template <typename T>
stack<T>::stack(stack<T> const& copy)
{
    std::lock_guard<std::mutex> a(copy.mtx);
    T* temp = new T[copy.array_size_];
    array_size_ = copy.array_size_;
    count_ = copy.count_;
    array_ = temp;
    try
    {
        std::copy(copy.array_, copy.array_ + count_, array_);
    }
    catch( ... )
    {
        std::cerr << "Error! Try again!" << std::endl;
        delete[] array_;
    }
}

template <typename T>
stack<T>& stack<T>::operator=(stack<T> const& other) noexcept
{
    std::lock(mtx,other.mtx);
    if (this != &other)
    {
        stack<T> temp(other);
        swap(temp);
    }
    mtx.unlock();
    other.mtx.unlock();
    return *this;
}

template <typename T>
void stack<T>::swap(stack<T>& other) noexcept
{
    std::lock_guard<std::mutex> a(mtx);
    std::swap(array_, other.array_);
    std::swap(array_size_, other.array_size_);
    std::swap(count_, other.count_);
}

template <typename T>
void stack<T>::push(T const& value)
{
    std::lock_guard<std::mutex> a(mtx);
    if (array_size_ == count_)
    {
        size_t size = array_size_;
        if (size == 0)
            size = 1;
        else
            size = array_size_ * 2;

        T* temp = new T[size];
        std::copy(array_, array_ + count_, temp);

        array_size_ = size;
        delete[] array_;
        array_ = temp;
    }

    array_[count_] = value;
    ++count_;
}

template <typename T>
auto stack<T>::pop() -> std::shared_ptr<T>
{
    std::lock_guard<std::mutex> lock(std::mutex mutex_);
    auto top = std::make_shared<T>(array_[count_ - 1]);
    if (count_ == 0)
        throw std::logic_error("Stack is empty");
    --count_;
    return top;
}
