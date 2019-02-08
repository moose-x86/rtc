#include <fstream>
#include <stdexcept>
#include <boost/property_tree/xml_parser.hpp>
#include <iostream>
#include <string>
#include <cassert>

#include "brs.hpp"

namespace rtc
{

brs::brs(const std::string& files)
{
  using namespace boost::property_tree;
  check_if_all_files_exist({files});

  for(const auto& p : {files})
  {
    ptree tree;
    read_xml(p, tree);

    load_points(tree);
    load_triangles(tree);
    load_attributes(tree);
    load_lights(tree);
    load_camera_info(tree);
  }

  check_all_invariants();
}

void brs::check_if_all_files_exist(const std::initializer_list<std::string>& files)
{
  for(const auto& s : files)
  {
    std::ifstream f{s};
    if(!f.good())
      throw std::runtime_error("file " + s + " can not be openned");
  }
}

void brs::check_all_invariants()
{
  if(points.empty() || triangles.empty() || materials.empty() || lights.empty())
  {
    throw std::runtime_error("There not all information");
  }
}

void brs::load_points(const boost::property_tree::ptree& tree)
{
  using namespace boost::property_tree;
  auto nodes = tree.get_child("model.points");

  points.reserve(nodes.get<std::size_t>("<xmlattr>.size"));

  for(const auto& n : nodes)
  {
    if(n.first == "point")
    {
      points.emplace_back(n.second.get<rtc_float>("<xmlattr>.x"),
                          n.second.get<rtc_float>("<xmlattr>.y"),
                          n.second.get<rtc_float>("<xmlattr>.z"));
    }
  }
}

void brs::load_triangles(const boost::property_tree::ptree& tree)
{
  using namespace boost::property_tree;
  auto nodes = tree.get_child("model.triangles");

  triangles.reserve(nodes.get<std::size_t>("<xmlattr>.size"));
  material_id.reserve(nodes.get<std::size_t>("<xmlattr>.size"));

  for(const auto& n : nodes)
  {
    if(n.first == "triangle")
    {
      triangles.emplace_back(n.second.get<std::uint32_t>("<xmlattr>.v1"),
                             n.second.get<std::uint32_t>("<xmlattr>.v2"),
                             n.second.get<std::uint32_t>("<xmlattr>.v3"));
      material_id.emplace_back(n.second.get<std::uint32_t>("<xmlattr>.attribute"));
    }
  }

  normals = std::vector<rtc::math_vector>(triangles.size());
}

void brs::load_lights(const boost::property_tree::ptree& tree)
{
  using namespace boost::property_tree;
  auto nodes = tree.get_child("model.lights");

  lights.reserve(nodes.get<std::size_t>("<xmlattr>.size"));
  for(const auto& n : nodes)
  {
    if(n.first == "light")
    {
      rtc::light l{};

      l.position = rtc::math_point{
            n.second.get<rtc_float>("position.<xmlattr>.x"),
            n.second.get<rtc_float>("position.<xmlattr>.y"),
            n.second.get<rtc_float>("position.<xmlattr>.z")
      };

      l.sampling = n.second.get<std::uint32_t>("sampling");
      l.radius   = n.second.get<rtc_float>("radius");
      l.light_color = rtc::color{
          n.second.get<rtc_float>("color.r"),
          n.second.get<rtc_float>("color.g"),
          n.second.get<rtc_float>("color.b"),
      };

      lights.push_back(l);
    }
    else if(n.first == "ambient")
    {
      ambient = rtc::color{
          n.second.get<rtc_float>("color.r"),
          n.second.get<rtc_float>("color.g"),
          n.second.get<rtc_float>("color.b")
      };
    }
  }
}

void brs::load_attributes(const boost::property_tree::ptree& tree)
{
  using namespace boost::property_tree;
  auto nodes = tree.get_child("model.attributes");

  materials.reserve(nodes.get<std::size_t>("<xmlattr>.size"));
  for(const auto& n : nodes)
  {
    if(n.first == "attribute")
    {
      rtc::surface_material sm{};

      for(auto& nn : n.second.get_child(""))
      {
        if(nn.first == "p")
        {
          auto name = nn.second.get<std::string>("<xmlattr>.name");

          if(name == "kd")
            sm.kd = std::stof(nn.second.data());

          if(name == "ks")
            sm.ks = std::stof(nn.second.data());

          if(name == "kts")
            sm.kts = std::stof(nn.second.data());

          if(name == "ktd")
            sm.ktd = std::stof(nn.second.data());

          if(name == "ka")
            sm.ka = std::stof(nn.second.data());

          if(name == "kf")
            sm.kf = std::stof(nn.second.data());

          if(name == "gs")
            sm.gs = std::stof(nn.second.data());

          if(name == "gm")
            sm.gm = std::stof(nn.second.data());

          if(name == "ft")
            sm.ft = std::stof(nn.second.data());

          if(name == "eta")
            sm.eta = std::stof(nn.second.data());

          if(name == "selfLuminance")
            sm.selfLuminance = std::stof(nn.second.data());

          if(name == "heatLam")
            sm.heatLam = std::stof(nn.second.data());

          if(name == "oneSheet")
            sm.oneSheet = nn.second.data() == "true" ? true : false;

          if(name == "mirror")
            sm.mirror = nn.second.data() == "true" ? true : false;

          if(name == "reflection")
            sm.reflection = nn.second.data() == "true" ? true : false;

          if(name == "indirect")
            sm.indirect = nn.second.data() == "true" ? true : false;

          if(name == "shadowcast")
            sm.shadowcast = nn.second.data() == "true" ? true : false;

          if(name == "shadowfall")
            sm.shadowfall = nn.second.data() == "true" ? true : false;

          sm.shading = surface_material::shading_type::guard;
        }
        else if(nn.first == "color")
        {
          sm.material_color = rtc::color{
            nn.second.get<rtc_float>("r"),
            nn.second.get<rtc_float>("g"),
            nn.second.get<rtc_float>("b")
          };
        }
      }

      materials.push_back(sm);
    }
  }
}

void brs::load_camera_info(const boost::property_tree::ptree& tree)
{
  using namespace boost::property_tree;

  std::vector<rtc::camera> cams;
  for(const auto& n : tree.get_child("model.observers"))
  {
    if(n.first == "camera")
    {
      rtc::camera c{};

      c.view_point = {
             n.second.get<rtc_float>("position.<xmlattr>.x"),
             n.second.get<rtc_float>("position.<xmlattr>.y"),
             n.second.get<rtc_float>("position.<xmlattr>.z")};

      c.screen.resolution.x = n.second.get<std::uint32_t>("screen.resolution.<xmlattr>.x");
      c.screen.resolution.y = n.second.get<std::uint32_t>("screen.resolution.<xmlattr>.y");

      for(const auto& nn : n.second.get_child("screen"))
      {
        if(nn.first == "corner")
        {
          [&c, &nn]() -> rtc::math_point& {
            if(nn.second.get<std::string>("<xmlattr>.position") == "left upper")
              return c.screen.surface.upper_left_corner;

            if(nn.second.get<std::string>("<xmlattr>.position") == "right upper")
              return c.screen.surface.upper_right_corner;

            if(nn.second.get<std::string>("<xmlattr>.position") == "left lower")
              return c.screen.surface.lower_left_corner;

            throw std::runtime_error("Wrong corner position given in xml!");
          }() = {
              nn.second.get<rtc_float>("<xmlattr>.x"),
              nn.second.get<rtc_float>("<xmlattr>.y"),
              nn.second.get<rtc_float>("<xmlattr>.z")};
        }
      }

      cams.push_back(c);
    }
  }

  const auto id = tree.get<std::uint32_t>("model.observers.<xmlattr>.active");

  if(id < cams.size() && !cams.empty())
  {
    optical_system = cams[id];
  }
  else
  {
    throw std::runtime_error("Wrong cam data");
  }
}

}
