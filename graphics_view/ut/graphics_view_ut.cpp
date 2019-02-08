#include "gtest/gtest.h"
#include "graphics_view.hpp"
#include "brs.hpp"
#include "blend.hpp"
#include <memory>
#include "rtc_log.hpp"

namespace rtc
{
namespace ut
{

TEST(graphics_view_ut, DISABLED_basic_move)
{
  auto data = std::make_shared<rtc::brs>("room.xml");
  rtc::graphics_view<> g{data};

  auto image_view{std::move(g)};
}

TEST(graphics_view_ut, basic_room_test)
{
  auto data = std::make_shared<rtc::brs>("room.xml");
  rtc::graphics_view<> gv{data};

  auto start = std::chrono::high_resolution_clock::now();
  auto b = gv.bitmap();
  auto end = std::chrono::high_resolution_clock::now();
  auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  RELEASE << "basic_test_ut: time of computation : " << t.count() << "[ms]";

  b.save("room.bmp");
}

TEST(graphics_view_ut, kitchen_test)
{
  auto data = std::make_shared<rtc::brs>("kitchen.xml");
  rtc::graphics_view<> gv{data};

  auto start = std::chrono::high_resolution_clock::now();
  auto b = gv.bitmap();
  auto end = std::chrono::high_resolution_clock::now();
  auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  RELEASE << "basic_test_ut: time of computation : " << t.count() << "[ms]";

  b.save("kitchen.bmp");
}

TEST(graphics_view_ut, bottles_test)
{
  auto data = std::make_shared<rtc::brs>("bottles.xml");
  rtc::graphics_view<> gv{data};

  auto start = std::chrono::high_resolution_clock::now();
  auto b = gv.bitmap();
  auto end = std::chrono::high_resolution_clock::now();
  auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  RELEASE << "basic_test_ut: time of computation : " << t.count() << "[ms]";

  b.save("bottles.bmp");
}

TEST(graphics_view_ut, sun_room_test)
{
  auto data = std::make_shared<rtc::brs>("sun_room.xml");
  rtc::graphics_view<> gv{data};

  auto start = std::chrono::high_resolution_clock::now();
  auto b = gv.bitmap();
  auto end = std::chrono::high_resolution_clock::now();
  auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  RELEASE << "basic_test_ut: time of computation : " << t.count() << "[ms]";

  b.save("sun_room.bmp");
}

TEST(graphics_view_ut, coffee_tables_test)
{
  auto data = std::make_shared<rtc::brs>("coffee_tables.xml");
  rtc::graphics_view<> gv{data};

  auto start = std::chrono::high_resolution_clock::now();
  auto b = gv.bitmap();
  auto end = std::chrono::high_resolution_clock::now();
  auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  RELEASE << "basic_test_ut: time of computation : " << t.count() << "[ms]";

  b.save("coffee_tables.bmp");
}

TEST(graphics_view_ut, road_test)
{
  auto data = std::make_shared<rtc::brs>("ulica.xml");
  rtc::graphics_view<> gv{data};

  auto start = std::chrono::high_resolution_clock::now();
  auto b = gv.bitmap();
  auto end = std::chrono::high_resolution_clock::now();
  auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  RELEASE << "basic_test_ut: time of computation : " << t.count() << "[ms]";

  b.save("road.bmp");
}

/*
TEST(graphics_view_ut, cornell_test)
{
  auto data = std::make_shared<rtc::brs>("cornell_box.xml");
  rtc::graphics_view<> gv{data};

  auto start = std::chrono::high_resolution_clock::now();
  auto b = gv.bitmap();
  auto end = std::chrono::high_resolution_clock::now();
  auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  RELEASE << "basic_test_ut: time of computation : " << t.count() << "[ms]";

  b.save("brs_cornell_box.bmp");
}
*/

TEST(graphics_view_ut, collada_cornell_box_test)
{
  auto data = std::make_shared<rtc::blend>("cornell_box.dae");
  rtc::graphics_view<> gv{data};

  auto start = std::chrono::high_resolution_clock::now();
  auto b = gv.bitmap();
  auto end = std::chrono::high_resolution_clock::now();
  auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  RELEASE << "basic_test_ut: time of computation : " << t.count() << "[ms]";

  b.save("cornell_box.bmp");
}

TEST(graphics_view_ut, collada_mech_room_test)
{
  auto data = std::make_shared<rtc::blend>("Mech_Room.dae");
  rtc::graphics_view<> gv{data};

  auto start = std::chrono::high_resolution_clock::now();
  auto b = gv.bitmap();
  auto end = std::chrono::high_resolution_clock::now();
  auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  RELEASE << "basic_test_ut: time of computation : " << t.count() << "[ms]";

  b.save("Mech_Room.bmp");
}

}
}
