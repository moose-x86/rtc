cmake_minimum_required(VERSION 3.9)

project(rtc_data_model CXX)

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

enable_testing()

FILE(GLOB RTC_DATA_MODEL_SOURCES
     source/*.cpp
)
add_library(rtc_data_model ${RTC_DATA_MODEL_SOURCES})
set_target_properties(rtc_data_model PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
target_include_directories(rtc_data_model PUBLIC include/)
target_link_libraries(rtc_data_model PUBLIC rtc_math) #why private triggers error

FILE(GLOB RTC_DATA_MODEL_TESTS
     ut/*.cpp
)

add_executable(rtc_data_model_ut ${RTC_DATA_MODEL_TESTS})

target_include_directories(rtc_data_model_ut SYSTEM PRIVATE ${GMOCK_INCLUDE_DIR} ${GTEST_INCLUDE_DIR})

set_target_properties(rtc_data_model_ut PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

if(${ENABLE_COVERAGE})
  add_compile_options(--coverage -O0)
  link_libraries(--coverage)
endif()

target_link_libraries(rtc_data_model_ut PRIVATE rtc_data_model rtc_math gmock_main)
add_test(rtc_data_model_ut ${CMAKE_BINARY_DIR}/data_model/rtc_data_model_ut --gtest_color=yes)
