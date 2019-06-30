#include "scene_model.hpp"
#include <memory>

namespace rtc
{

struct blend : public rtc::scene_model
{
public:
  explicit blend(const std::string&);
  ~blend();

private:
  struct blend_impl;
  std::unique_ptr<blend_impl> blendimpl;  
};

}