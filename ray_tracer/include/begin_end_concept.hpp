#pragma once

#include <type_traits>
#include "math_ray.hpp"

namespace rtc
{

template<typename T, typename = void>
struct has_ray_begin_and_end : std::false_type {};

template<typename T>
struct has_ray_begin_and_end<T,
                             std::void_t<decltype(
                                           std::declval<const T&>().cbegin(std::declval<const math_ray&>()) ==
                                           std::declval<const T&>().cend(std::declval<const math_ray&>())
                                         ),
                                         decltype(
                                           std::declval<const T&>().cbegin(std::declval<const math_ray&>()) !=
                                           std::declval<const T&>().cend(std::declval<const math_ray&>())
                                         ),
                                         decltype(
                                           std::declval<const T&>().cbegin(std::declval<const math_ray&>()) =
                                           std::declval<const T&>().cend(std::declval<const math_ray&>())
                                         ),
                                         decltype(
                                           std::declval<const T&>().cbegin(std::declval<const math_ray&>()).operator++()
                                         ),
                                         decltype(
                                           std::declval<const T&>().cbegin(std::declval<const math_ray&>()).operator*()
                                         )
                              >
                             > : std::true_type {};

template<typename T>
constexpr bool has_ray_begin_and_end_v{has_ray_begin_and_end<T>::value};

}
