//
// Created by np on 22.5.26.
//

#ifndef CHESSL_STACK_VECTOR_H
#define CHESSL_STACK_VECTOR_H

#include <algorithm>
#include <array>

// TODO: Benchmark vector vs StackVector performance

namespace chess {

template<class T, std::size_t Size>
class StackVector {
    std::array<T, Size> arr_{};
    typedef typename std::array<T, Size>::iterator iterator;
    iterator arr_end_;

public:
    //    constexpr explicit StackVector(std::array<T, Size> arr) noexcept : arr_{arr}, arr_end_{arr_.begin()} {
    //    }

    constexpr StackVector() noexcept : arr_end_{arr_.begin()} {
    }

    constexpr StackVector(const StackVector &other) noexcept {
        arr_end_ = arr_.begin() + other.size();
        std::copy((iterator) other.arr_.begin(), other.arr_end_, arr_.begin());
    }

    ~StackVector() = default;

    // TODO: Implement proper move constructor
//    constexpr StackVector &operator=(StackVector &other) noexcept {
//        if (this == &other) {
//            return *this;
//        }
//
//        *this = *other;
////        arr_ = other.arr_;
////        arr_end_ = other.arr_end_;
//        return *this;
//    }

    constexpr StackVector &operator=(const StackVector &other) noexcept {
        if (this == &other) {
            return *this;
        }

        arr_end_ = arr_.begin() + other.size();
        std::copy((iterator) other.arr_.begin(), other.arr_end_, arr_.begin());
        return *this;
    }

    [[nodiscard]] constexpr T operator[](std::size_t idx) const noexcept {
        return arr_[idx];
    }

    [[nodiscard]] constexpr T &operator[](std::size_t idx) noexcept {
        return arr_[idx];
    }

    [[nodiscard]] constexpr T &front() noexcept {
        return arr_.front();
    }

    [[nodiscard]] constexpr T front() const noexcept {
        return arr_.front();
    }

    [[nodiscard]] constexpr T &back() noexcept {
        return *arr_end_;
    }

    [[nodiscard]] constexpr T back() const noexcept {
        return *arr_end_;
    }

    [[nodiscard]] constexpr T *begin() {
        return arr_.begin();
    }

    [[nodiscard]] constexpr const T *begin() const noexcept {
        return arr_.begin();
    }

    [[nodiscard]] constexpr T *end() noexcept {
        return arr_end_;
    }

    [[nodiscard]] constexpr const T *end() const noexcept {
        return arr_end_;
    }

    [[nodiscard]] constexpr bool empty() const noexcept {
        return arr_end_ == arr_.begin();
    }

    //    [[nodiscard]] constexpr explicit operator bool() const noexcept {
    //        return arr_.begin() - arr_end_;
    //    }

    [[nodiscard]] constexpr std::size_t size() const noexcept {
        return arr_end_ - arr_.begin();
    }

    constexpr T &push_back(const T &value) noexcept {
        return (*(arr_end_++) = value);
    }

    template<class... Args>
    constexpr T &emplace_back(Args &&...args) noexcept {
        return push_back(std::forward<Args>(args)...);
    }

    constexpr void clear() noexcept {
        arr_end_ = arr_.begin();
    }
};

}

#endif//CHESSL_STACK_VECTOR_H
