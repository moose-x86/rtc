cmake_minimum_required(VERSION 3.10)

project(rtc_kd_tree CXX)

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


find_package(Boost REQUIRED)

enable_testing()

FILE(GLOB RTC_KDTREE_SOURCES
     source/*.cpp
)
add_library(rtc_kd_tree ${RTC_KDTREE_SOURCES})
set_target_properties(rtc_kd_tree PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_include_directories(rtc_kd_tree PUBLIC include/ ${Boost_INCLUDE_DIRS})

FILE(GLOB RTC_KDTREE_UT
     ut/*.cpp
)
add_executable(kd_tree_ut ${RTC_KDTREE_UT})
set_target_properties(kd_tree_ut PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

if(${ENABLE_COVERAGE})
  add_compile_options(--coverage -O0)
  link_libraries(--coverage)
endif()

target_include_directories(kd_tree_ut SYSTEM PRIVATE ${GMOCK_INCLUDE_DIR} ${GTEST_INCLUDE_DIR})

target_link_libraries(rtc_kd_tree PUBLIC rtc_core rtc_ray_tracer rtc_data_model rtc_math)
target_link_libraries(kd_tree_ut PRIVATE rtc_core rtc_kd_tree input_model gmock_main)
add_test(kd_storage_ut ${CMAKE_BINARY_DIR}/kd_tree/kd_tree_ut --gtest_color=yes)

file(COPY ${CMAKE_SOURCE_DIR}/assets/brs_xml/room.xml DESTINATION ${CMAKE_BINARY_DIR}/kd_tree/)
file(COPY ${CMAKE_SOURCE_DIR}/assets/brs_xml/kitchen.xml DESTINATION ${CMAKE_BINARY_DIR}/kd_tree/)
file(COPY ${CMAKE_SOURCE_DIR}/assets/brs_xml/ulica.xml DESTINATION ${CMAKE_BINARY_DIR}/kd_tree/)
file(COPY ${CMAKE_SOURCE_DIR}/assets/brs_xml/cornell_box.xml DESTINATION ${CMAKE_BINARY_DIR}/kd_tree/)
