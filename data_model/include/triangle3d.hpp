#pragma once

#include <cstdint>
#include <array>
namespace rtc
{

struct triangle3d
{
  using value_type = std::uint32_t;
  constexpr triangle3d(value_type t1, value_type t2, value_type t3) noexcept : i{t1, t2, t3}
  {}

  [[nodiscard]] constexpr auto vertex_a() const noexcept {return i[0];}
  [[nodiscard]] constexpr auto vertex_b() const noexcept {return i[1];}
  [[nodiscard]] constexpr auto vertex_c() const noexcept {return i[2];}

private:
  std::array<value_type, 3> i{};
};

inline bool operator==(const triangle3d& t1, const triangle3d& t2) noexcept
{
  return (t1.vertex_a() == t2.vertex_a()) && (t1.vertex_b() == t2.vertex_b()) && (t1.vertex_c() == t2.vertex_c());
}

inline bool operator!=(const triangle3d& t1, const triangle3d& t2) noexcept
{
  return !(t1 == t2);
}

inline std::ostream& operator<<(std::ostream& s, const triangle3d& t) noexcept
{
  return s << "[" << t.vertex_a() << ", " << t.vertex_b() << ", " << t.vertex_c() << "]t";
}

}
