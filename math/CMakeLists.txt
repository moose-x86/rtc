cmake_minimum_required(VERSION 3.10)

project(rtc_math CXX)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Ox")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "-Ofast ${SSE4_FLAG}")        ## Optimize
    #set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary
   else()
    set(CMAKE_CXX_FLAGS "-ggdb -O0")        ## no Optimize
  endif()
endif()
enable_testing()

FILE(GLOB RTC_MATH_SOURCES
     source/*.cpp
)
add_library(rtc_math ${RTC_MATH_SOURCES})

set_target_properties(rtc_math PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_include_directories(rtc_math PUBLIC include/)
target_link_libraries(rtc_math PUBLIC rtc_core) #why private triggers error


FILE(GLOB RTC_MATH_TESTS
     ut/*.cpp
)

add_executable(rtc_math_ut ${RTC_MATH_TESTS})
set_target_properties(rtc_math_ut PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
target_include_directories(rtc_math_ut SYSTEM PRIVATE ${GMOCK_INCLUDE_DIR} ${GTEST_INCLUDE_DIR})

if(${ENABLE_COVERAGE})
  add_compile_options(--coverage -O0)
  link_libraries(--coverage)
endif()

target_link_libraries(rtc_math_ut PRIVATE rtc_math gmock_main)
add_test(rtc_math_ut ${CMAKE_BINARY_DIR}/math/rtc_math_ut --gtest_color=yes)
