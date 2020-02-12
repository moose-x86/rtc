#include <memory>

#include "brs.hpp"
#include "collada_format.hpp"
#include "gtest/gtest.h"
#include "render_engine.hpp"
#include "rtc_log.hpp"

namespace rtc::ut
{
TEST(render_engine_ut, DISABLED_basic_move)
{
  auto data = std::make_shared<rtc::brs>("room.xml");
  rtc::render_engine<> g{data};

  auto image_view{std::move(g)};
}

TEST(render_engine_ut, basic_room_test)
{
  auto data = std::make_shared<rtc::brs>("room.xml");
  rtc::render_engine<> gv{data};

  auto start = std::chrono::high_resolution_clock::now();
  auto b = gv.bitmap();
  auto end = std::chrono::high_resolution_clock::now();
  auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  RELEASE << "basic_test_ut: time of computation : " << t.count() << "[ms]";

  b.save("room.bmp");
}

TEST(render_engine_ut, kitchen_test)
{
  auto data = std::make_shared<rtc::brs>("kitchen.xml");
  rtc::render_engine<> gv{data};

  auto start = std::chrono::high_resolution_clock::now();
  auto b = gv.bitmap();
  auto end = std::chrono::high_resolution_clock::now();
  auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  RELEASE << "basic_test_ut: time of computation : " << t.count() << "[ms]";

  b.save("kitchen.bmp");
}

TEST(render_engine_ut, bottles_test)
{
  auto data = std::make_shared<rtc::brs>("bottles.xml");
  rtc::render_engine<> gv{data};

  auto start = std::chrono::high_resolution_clock::now();
  auto b = gv.bitmap();
  auto end = std::chrono::high_resolution_clock::now();
  auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  RELEASE << "basic_test_ut: time of computation : " << t.count() << "[ms]";

  b.save("bottles.bmp");
}

TEST(render_engine_ut, sun_room_test)
{
  auto data = std::make_shared<rtc::brs>("sun_room.xml");
  rtc::render_engine<> gv{data};

  auto start = std::chrono::high_resolution_clock::now();
  auto b = gv.bitmap();
  auto end = std::chrono::high_resolution_clock::now();
  auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  RELEASE << "basic_test_ut: time of computation : " << t.count() << "[ms]";

  b.save("sun_room.bmp");
}

TEST(render_engine_ut, coffee_tables_test)
{
  auto data = std::make_shared<rtc::brs>("coffee_tables.xml");
  rtc::render_engine<> gv{data};

  auto start = std::chrono::high_resolution_clock::now();
  auto b = gv.bitmap();
  auto end = std::chrono::high_resolution_clock::now();
  auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  RELEASE << "basic_test_ut: time of computation : " << t.count() << "[ms]";

  b.save("coffee_tables.bmp");
}

TEST(render_engine_ut, DISABLED_road_test)
{
  auto data = std::make_shared<rtc::brs>("ulica.xml");
  rtc::render_engine<> gv{data};

  auto start = std::chrono::high_resolution_clock::now();
  auto b = gv.bitmap();
  auto end = std::chrono::high_resolution_clock::now();
  auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  RELEASE << "basic_test_ut: time of computation : " << t.count() << "[ms]";

  b.save("road.bmp");
}

TEST(render_engine_ut, DISABLED_cornell_test)
{
  auto data = std::make_shared<rtc::brs>("cornell_box.xml");
  rtc::render_engine<> gv{data};

  auto start = std::chrono::high_resolution_clock::now();
  auto b = gv.bitmap();
  auto end = std::chrono::high_resolution_clock::now();
  auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  RELEASE << "basic_test_ut: time of computation : " << t.count() << "[ms]";

  b.save("brs_cornell_box.bmp");
}

TEST(render_engine_ut, collada_cornell_box_test)
{
  auto data = std::make_shared<rtc::collada_format>("cornell_box.dae");
  rtc::render_engine<> gv{data};

  auto start = std::chrono::high_resolution_clock::now();
  auto b = gv.bitmap();
  auto end = std::chrono::high_resolution_clock::now();
  auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  RELEASE << "basic_test_ut: time of computation : " << t.count() << "[ms]";

  b.save("cornell_box.bmp");
}

TEST(render_engine_ut, collada_mech_room_test)
{
  auto data = std::make_shared<rtc::collada_format>("Mech_Room.dae");
  rtc::render_engine<> gv{data};

  auto start = std::chrono::high_resolution_clock::now();
  auto b = gv.bitmap();
  auto end = std::chrono::high_resolution_clock::now();
  auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  RELEASE << "basic_test_ut: time of computation : " << t.count() << "[ms]";

  b.save("Mech_Room.bmp");
}

}  // namespace rtc::ut
