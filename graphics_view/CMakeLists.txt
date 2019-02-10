cmake_minimum_required(VERSION 3.9)

project(rtc_image_generation CXX)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Ox")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
  if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "-Ofast ${SSE4_FLAG} -pedantic")        ## Optimize
    #set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary
   else()
    set(CMAKE_CXX_FLAGS "-ggdb -O0 -pedantic")        ## no Optimize
  endif()
endif()

find_package(Boost COMPONENTS thread system fiber context)

enable_testing()

#FILE(GLOB RTC_image_generation_SOURCES
     #source/*.cpp
#)
add_library(rtc_image_generation INTERFACE)

target_include_directories(rtc_image_generation INTERFACE include/ ${Boost_INCLUDE_DIRS})
target_link_libraries(rtc_image_generation INTERFACE rtc_rt_algorithms rtc_rt_service rtc_kd_tree rtc_data_model rtc_core) #why private triggers error
target_link_libraries(rtc_image_generation INTERFACE ${Boost_THREAD_LIBRARY_RELEASE} ${Boost_SYSTEM_LIBRARY_RELEASE} ${Boost_FIBER_LIBRARY_RELEASE} ${Boost_CONTEXT_LIBRARY_RELEASE}) #why private triggers error


FILE(GLOB RTC_image_generation_TESTS
     ut/*.cpp
)

add_executable(rtc_image_generation_ut ${RTC_image_generation_TESTS})
set_target_properties(rtc_image_generation_ut PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_include_directories(rtc_image_generation_ut SYSTEM PRIVATE ${GMOCK_INCLUDE_DIR} ${GTEST_INCLUDE_DIR})

if(${ENABLE_COVERAGE})
  add_compile_options(--coverage -O0)
  link_libraries(--coverage)
endif()

target_link_libraries(rtc_image_generation_ut PRIVATE input_model rtc_image_generation gmock_main)
add_test(rtc_image_generation_ut ${CMAKE_BINARY_DIR}/graphics_view/rtc_image_generation_ut --gtest_color=yes)

file(COPY ${CMAKE_SOURCE_DIR}/assets/brs_xml/room.xml DESTINATION ${CMAKE_BINARY_DIR}/graphics_view/)
file(COPY ${CMAKE_SOURCE_DIR}/assets/brs_xml/ulica.xml DESTINATION ${CMAKE_BINARY_DIR}/graphics_view/)
file(COPY ${CMAKE_SOURCE_DIR}/assets/brs_xml/sun_room.xml DESTINATION ${CMAKE_BINARY_DIR}/graphics_view/)
file(COPY ${CMAKE_SOURCE_DIR}/assets/brs_xml/coffee_tables.xml DESTINATION ${CMAKE_BINARY_DIR}/graphics_view/)
#file(COPY ${CMAKE_SOURCE_DIR}/assets/brs_xml/cornell_box.xml DESTINATION ${CMAKE_BINARY_DIR}/graphics_view/)
file(COPY ${CMAKE_SOURCE_DIR}/assets/brs_xml/kitchen.xml DESTINATION ${CMAKE_BINARY_DIR}/graphics_view/)
file(COPY ${CMAKE_SOURCE_DIR}/assets/brs_xml/bottles.xml DESTINATION ${CMAKE_BINARY_DIR}/graphics_view/)
file(COPY ${CMAKE_SOURCE_DIR}/assets/collada/cornell_box.dae DESTINATION ${CMAKE_BINARY_DIR}/graphics_view/)
file(COPY ${CMAKE_SOURCE_DIR}/assets/collada/Mech_Room.dae DESTINATION ${CMAKE_BINARY_DIR}/graphics_view/)
