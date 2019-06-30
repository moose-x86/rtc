#pragma once

#include <map>
#include <set>
#include <vector>

namespace std {

template <typename T>
auto operator<<(std::ostream &ss, const std::vector<T> &v) noexcept -> std::ostream & {
  ss << "{ ";

  if (not v.empty()) {
    ss << v.front();
    for (auto e = std::next(v.begin()); e != v.end(); ++e)
      ss << ", " << *e;
  }

  return ss << " }";
}

template <typename T>
auto operator<<(std::ostream &ss, const std::set<T> &v) noexcept -> std::ostream & {
  ss << "{ ";

  if (not v.empty()) {
    ss << *v.begin();
    for (auto e = std::next(v.begin()); e != v.end(); ++e)
      ss << ", " << *e;
  }

  return ss << " }";
}

template <typename K, typename V>
auto operator<<(std::ostream &ss, const std::map<K, V> &v) noexcept -> std::ostream & {
  ss << "{ ";

  if (not v.empty()) {
    ss << "{ " << v.begin()->first << " = " << v.begin()->second << " }";
    for (auto e = std::next(v.begin()); e != v.end(); ++e)
      ss << ",\n{ " << e->first << " = " << e->second << " }";
  }

  return ss << " }";
}

} // namespace std
