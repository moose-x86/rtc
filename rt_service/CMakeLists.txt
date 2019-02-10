cmake_minimum_required(VERSION 3.10)

project(rtc_rt_service CXX)

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

find_package(Boost COMPONENTS thread system)

enable_testing()

#FILE(GLOB RTC_RT_SERVICE_SOURCES
     #source/*.cpp
#)
add_library(rtc_rt_service INTERFACE)

target_include_directories(rtc_rt_service INTERFACE include/ ${Boost_INCLUDE_DIRS})
target_link_libraries(rtc_rt_service INTERFACE rtc_kd_tree) #why private triggers error
target_link_libraries(rtc_rt_service INTERFACE ${Boost_THREAD_LIBRARY_RELEASE} ${Boost_SYSTEM_LIBRARY_RELEASE})#why private triggers error


FILE(GLOB RTC_RT_SERVICE_TESTS
     ut/*.cpp
)

add_executable(rtc_rt_service_ut ${RTC_RT_SERVICE_TESTS})
set_target_properties(rtc_rt_service_ut PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_include_directories(rtc_rt_service_ut SYSTEM PRIVATE ${GMOCK_INCLUDE_DIR} ${GTEST_INCLUDE_DIR})

if(${ENABLE_COVERAGE})
  add_compile_options(--coverage -O0)
  link_libraries(--coverage)
endif()

target_link_libraries(rtc_rt_service_ut PRIVATE input_model rtc_rt_service gmock_main)
add_test(rtc_rt_service_ut ${CMAKE_BINARY_DIR}/rt_service/rtc_rt_service_ut --gtest_color=yes)

file(COPY ${CMAKE_SOURCE_DIR}/assets/brs_xml/room.xml DESTINATION ${CMAKE_BINARY_DIR}/rt_service/)
