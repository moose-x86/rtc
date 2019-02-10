cmake_minimum_required(VERSION 3.9)

project(input_model CXX)

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
add_library(input_model
            source/brs.cpp
            source/blend.cpp
            )

set_target_properties(input_model PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_include_directories(input_model PUBLIC include/ ${Boost_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIR})

add_executable(input_model_ut
               ut/brs_ut.cpp
               ut/blender_ut.cpp
               )

if(${ENABLE_COVERAGE})
  add_compile_options(--coverage -O0)
  link_libraries(--coverage)
endif()

set_target_properties(input_model_ut PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_include_directories(input_model_ut SYSTEM PRIVATE ${GMOCK_INCLUDE_DIR} ${GTEST_INCLUDE_DIR})

target_link_libraries(input_model PRIVATE rtc_data_model assimp)
target_link_libraries(input_model_ut PRIVATE rtc_data_model input_model gmock_main)
add_test(input_model_ut ${CMAKE_BINARY_DIR}/input_model/input_model_ut --gtest_color=yes)

file(COPY ${CMAKE_SOURCE_DIR}/assets/brs_xml/room.xml DESTINATION ${CMAKE_BINARY_DIR}/input_model/)
file(COPY ${CMAKE_SOURCE_DIR}/assets/blender/Wind.blend DESTINATION ${CMAKE_BINARY_DIR}/input_model/)
