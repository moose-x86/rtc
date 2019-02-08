cmake_minimum_required(VERSION 3.10)

project(rtc_ray_tracer CXX)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Ox")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "-Ofast ${SSE4_FLAG} -pedantic")        ## Optimize
    #set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary
   else()
    set(CMAKE_CXX_FLAGS "-ggdb -O0 -pedantic")        ## no Optimize
  endif()
endif()

find_package(Boost REQUIRED)

enable_testing()

#FILE(GLOB RTC_ray_tracer_SOURCES
     #source/*.cpp
#)
add_library(rtc_ray_tracer INTERFACE)

target_include_directories(rtc_ray_tracer INTERFACE include/ ${Boost_INCLUDE_DIRS})
target_link_libraries(rtc_ray_tracer INTERFACE rtc_kd_tree) #why private triggers error
#target_link_libraries(rtc_ray_tracer INTERFACE ${Boost_THREAD_LIBRARY_RELEASE}) #why private triggers error


FILE(GLOB RTC_ray_tracer_TESTS
     ut/*.cpp
)

add_executable(rtc_ray_tracer_ut ${RTC_ray_tracer_TESTS})

if(${ENABLE_COVERAGE})
  add_compile_options(--coverage -O0)
  link_libraries(--coverage)
endif()

target_link_libraries(rtc_ray_tracer_ut PRIVATE input_model rtc_ray_tracer gmock_main)
add_test(rtc_ray_tracer_ut ${CMAKE_BINARY_DIR}/ray_tracer/rtc_ray_tracer_ut --gtest_color=yes)

file(COPY ${CMAKE_SOURCE_DIR}/assets/brs_xml/room.xml DESTINATION ${CMAKE_BINARY_DIR}/ray_tracer/)
