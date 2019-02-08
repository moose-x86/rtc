#pragma once

#include <vector>
#include <cassert>

#include "light.hpp"
#include "camera.hpp"
#include "math_point.hpp"
#include "triangle3d.hpp"
#include "surface_material.hpp"
#include "math_vector.hpp"

namespace rtc
{

/**
 * if we can put here other allocator with gpu mem, we could reuse this code
 * it looks like key to producing gpu code is having template struct with custom cpu and gpu allocator
 * strongly consider gpu mem allocator
 */
struct scene_model
{
  using lights_type = std::vector<light>;
  using points_type = std::vector<math_point>;
  using triangles_type = std::vector<triangle3d>;
  using materials_type = std::vector<surface_material>;
  using materialid_type =  std::vector<std::uint16_t>;

  color ambient{0.5, 0.5, 0.5};
  points_type points{}; //consider sorting this (x, y, z) < (x, y, z) as is it can better cache locality
  lights_type lights{};
  camera optical_system{};
  triangles_type triangles{};
  materials_type materials{};
  materialid_type material_id{};

  mutable std::vector<rtc::math_vector> normals;

  /**
   * std::vector<math_vector> normals_in_points;
   * struct Object
   * {
   *   std::vector<int> triangles;
   * };
   *
   * std::vector<Object> objects;
   */
};

}
