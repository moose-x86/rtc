cmake_minimum_required(VERSION 3.7)

project(rtc_core CXX)

enable_testing()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Ox")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
  if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "-Ofast -pedantic ${SS4_FLAG}")        ## Optimize
    #set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary
   else()
    set(CMAKE_CXX_FLAGS "-ggdb -O0 -pedantic")        ## no Optimize
  endif()
endif()

FILE(GLOB RTC_CORE_SOURCES
     source/*.cpp
)
add_library(rtc_core ${RTC_CORE_SOURCES})

set_target_properties(rtc_core PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_include_directories(rtc_core PUBLIC include/)
#target_link_libraries(rtc_core PUBLIC rtc_data_model) #why private triggers error


FILE(GLOB RTC_CORE_TESTS
     ut/*.cpp
)

add_executable(rtc_core_ut ${RTC_CORE_TESTS})

if(${ENABLE_COVERAGE})
  add_compile_options(--coverage -O0)
  link_libraries(--coverage)
endif()

target_link_libraries(rtc_core_ut PRIVATE rtc_core gmock_main)
add_test(rtc_core_ut ${CMAKE_BINARY_DIR}/core/rtc_core_ut --gtest_color=yes)
