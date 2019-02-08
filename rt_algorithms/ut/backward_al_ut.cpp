#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <memory>

#include "brs.hpp"
#include "rt_service.hpp"
#include "backward_al.hpp"
#include "distributed_ray_tracing.hpp"
#include "optical_camera_plane.hpp"

namespace ut
{

TEST(backward_al_ut, sun_room_test)
{
  const auto sc = std::make_shared<rtc::brs>("./sun_room.xml");
  const rtc::optical_camera_plane op{sc->optical_system};
  rtc::rt_service<> rt{sc};

  rtc::backward_al<
    void,
    rtc::distributed_ray_tracing_shadows<>,
    void
  > bt{sc};

  auto c = bt.make_color(op.emit_ray(830, 830), rtc::black, rt);

}

TEST(backward_al_ut, bottles_test)
{
  const auto sc = std::make_shared<rtc::brs>("./bottles.xml");

  sc->optical_system.screen.resolution.x = 1600;
  sc->optical_system.screen.resolution.y = 1200;

  const rtc::optical_camera_plane op{sc->optical_system};
  rtc::rt_service<> rt{sc};

  rtc::backward_al<
    void,
    rtc::distributed_ray_tracing_shadows<>,
    void
  > bt{sc};

  auto c = bt.make_color(op.emit_ray(597, 879), rtc::black, rt);
}

TEST(backward_al_ut, road_test)
{
  const auto sc = std::make_shared<rtc::brs>("./ulica.xml");

  sc->optical_system.screen.resolution.x = 160;
  sc->optical_system.screen.resolution.y = 100;

  const rtc::optical_camera_plane op{sc->optical_system};
  rtc::rt_service<> rt{sc};

  rtc::backward_al<
    void,
    rtc::distributed_ray_tracing_shadows<>,
    void
  > bt{sc};

  auto c = bt.make_color(op.emit_ray(9, 27), rtc::black, rt);
}

}
