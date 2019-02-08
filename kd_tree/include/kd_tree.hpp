#pragma once

#include <memory>
#include <tuple>

#include "bounding_edge.hpp"
#include "scene_model.hpp"
#include "math_vector.hpp"
#include "math_ray.hpp"
#include "ray_box_intersection_test.hpp"

namespace rtc
{

class kd_tree
{
  struct tree_node;
  using value_type = std::vector<std::uint32_t>;
  using edge_buffer_t = std::vector<rtc::bounding_edge_point>;
  using edge_buffer_array_t = std::array<edge_buffer_t, 3>;

  template<typename... T>
  using vector_tuple = std::tuple<std::vector<T>...>;

public:
  class const_iterator;

  rtc_hot kd_tree(const rtc::scene_model& sm);
  kd_tree(kd_tree&&) noexcept;
  kd_tree& operator=(kd_tree&&) noexcept;

  ~kd_tree();

  const_iterator cbegin(const rtc::math_ray&) const noexcept;
  const_iterator cend(const rtc::math_ray&) const noexcept;

private:
  ray_box_intersection_test bbox;
  std::unique_ptr<tree_node> root;

  rtc_hot void build_tree(std::unique_ptr<tree_node>& node,
                          rtc::bounding_box b,
                          std::vector<std::uint32_t> tr,
                          const std::vector<rtc::bounding_box>& primitive_bboxes,
                          edge_buffer_array_t& edges,
                          const std::uint32_t depth,
                          std::uint32_t bad_refines = 0);

  rtc_hot std::tuple<int, int, rtc_float, rtc_float> compute_node_split_paramters(
                                        edge_buffer_array_t& edges,
                                        const std::vector<std::uint32_t>& tr,
                                        const rtc::bounding_box& node_bbox,
                                        const std::vector<rtc::bounding_box>& primitive_bboxes);

  rtc_hot vector_tuple<std::uint32_t, std::uint32_t> split_triangles(
                                                      std::vector<std::uint32_t>&&,
                                                      const edge_buffer_array_t& edges,
                                                      const std::uint32_t best_axis,
                                                      const std::uint32_t best_offset);

};

}

#include "kd_tree_iterator.hpp"
