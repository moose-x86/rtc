#include "scene_model.hpp"
#include <memory>

namespace rtc
{

struct collada_format : public rtc::scene_model
{
public:
  explicit collada_format(const std::string&);
  ~collada_format();

private:
  struct collada_format_impl;
  std::unique_ptr<collada_format_impl> collada_formatimpl;
};

}