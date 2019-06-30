#pragma once

#include "scene_model.hpp"
#include <boost/property_tree/ptree.hpp>

namespace rtc
{

class brs : public scene_model
{
public:
  explicit brs(const std::string&);

private:
  void load_points(const boost::property_tree::ptree&);
  void load_lights(const boost::property_tree::ptree&);
  void load_attributes(const boost::property_tree::ptree&);
  void load_triangles(const boost::property_tree::ptree&);
  void load_camera_info(const boost::property_tree::ptree&);

  void check_if_all_files_exist(const std::initializer_list<std::string>& p);
  void check_all_invariants();
};

}
