#pragma once

#include <tuple>
#include <optional>
#include <type_traits>

namespace rtc
{

template<typename... T>
using optional_tuple = std::optional<std::tuple<T...>>;

template<typename... T>
auto make_optional_tuple(const bool cond, T&&... elements) noexcept ->
                                     optional_tuple< std::remove_cv_t<std::remove_reference_t<T>>... >
{
  return cond ? std::make_tuple(std::forward<T>(elements)...) : std::nullopt;
}

}

#define rtc_float float

#if  defined(__GNUC__) || defined(__clang__)
  #define rtc_likely(x) __builtin_expect((x), 1)
  #define rtc_unlikely(x) __builtin_expect((x), 0)
  #define rtc_hot [[gnu::hot]]
  #define rtc_pure [[gnu::pure]]
  #define rtc_unroll [[gnu::optimize("unroll-loops")]]
  #define rtc_inline [[gnu::always_inline]]
#else
  #define rtc_likely(x) (x)
  #define rtc_unlikely(x) (x)
  #define rtc_hot
  #define rtc_pure
  #define rtc_unroll
  #define rtc_inline
#endif

#ifdef _MSC_VER
  #define NOMINMAX
  #undef max
#endif
