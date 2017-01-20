#pragma once
#include <type_traits>
using std::underlying_type;

template<typename E>
constexpr auto to_integral(E e) -> typename underlying_type<E>::type {
    return static_cast<typename underlying_type<E>::type>(e);
}