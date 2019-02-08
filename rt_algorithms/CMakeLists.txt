cmake_minimum_required(VERSION 3.10)

project(rtc_rt_algorithms CXX)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Ox")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
  if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "--std=c++17 -Ofast ${SSE4_FLAG} -pedantic")        ## Optimize
    #set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary
   else()
    set(CMAKE_CXX_FLAGS "--std=c++17 -ggdb -O0 -pedantic")        ## no Optimize
  endif()
endif()

find_package(Boost REQUIRED)

enable_testing()

#FILE(GLOB RTC_rt_algorithms_SOURCES
     #source/*.cpp
#)
add_library(rtc_rt_algorithms INTERFACE)

target_include_directories(rtc_rt_algorithms INTERFACE include/ ${Boost_INCLUDE_DIRS})
target_link_libraries(rtc_rt_algorithms INTERFACE rtc_kd_tree) #why private triggers error
#target_link_libraries(rtc_rt_algorithms INTERFACE ${Boost_THREAD_LIBRARY_RELEASE}) #why private triggers error


FILE(GLOB RTC_rt_algorithms_TESTS
     ut/*.cpp
)

add_executable(rtc_rt_algorithms_ut ${RTC_rt_algorithms_TESTS})
target_include_directories(rtc_rt_algorithms_ut SYSTEM PRIVATE ${GMOCK_INCLUDE_DIR} ${GTEST_INCLUDE_DIR})


if(${ENABLE_COVERAGE})
  add_compile_options(--coverage -O0)
  link_libraries(--coverage)
endif()

target_link_libraries(rtc_rt_algorithms_ut PRIVATE input_model rtc_rt_algorithms rtc_rt_service gmock_main)
add_test(rtc_rt_algorithms_ut ${CMAKE_BINARY_DIR}/rt_algorithms/rtc_rt_algorithms_ut --gtest_color=yes)

file(COPY ${CMAKE_SOURCE_DIR}/assets/brs_xml/room.xml DESTINATION ${CMAKE_BINARY_DIR}/rt_algorithms/)
file(COPY ${CMAKE_SOURCE_DIR}/assets/brs_xml/sun_room.xml DESTINATION ${CMAKE_BINARY_DIR}/rt_algorithms/)
file(COPY ${CMAKE_SOURCE_DIR}/assets/brs_xml/bottles.xml DESTINATION ${CMAKE_BINARY_DIR}/rt_algorithms/)
