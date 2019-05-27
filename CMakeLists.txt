cmake_minimum_required(VERSION 3.10)
project(rtc CXX)

enable_testing()

set(GMOCK_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/googlemock/googlemock/include")
set(GTEST_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/googletest/googletest/include")

set(ASSIMP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/assimp/include")
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT CACHE INTERNAL FALSE)
SET(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Turn of assimp tests")
SET(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Turn off assimp tools")

set(ASSIMP_BUILD_COLLADA_IMPORTER ON)
SET(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libs")

if("${SSE4_DISABLE}" STREQUAL "yes")
  set(SSE4_FLAG "")
else()
  set(SSE4_FLAG "-msse4")
endif()


if("${BUILD_SHARED_LIBS}" STREQUAL "ON")
  set(Boost_USE_STATIC_LIBS OFF)
  set(Boost_USE_STATIC_RUNTIME OFF)

  string(REPLACE "/MTd" "/MDd" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
  string(REPLACE "/MT" "/MD" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
else()
  set(Boost_USE_STATIC_LIBS ON)
  set(Boost_USE_STATIC_RUNTIME ON)

  string(REPLACE "/MDd" "/MTd" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
  string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
endif()

if(${ENABLE_COVERAGE})
  add_compile_options(--coverage -O0)
  link_libraries(--coverage)
endif()

add_subdirectory(externals/googletest)
add_subdirectory(externals/assimp)
add_subdirectory(math/)
add_subdirectory(data_model/)
add_subdirectory(input_model/)
add_subdirectory(kd_tree/)
add_subdirectory(core/)
add_subdirectory(rt_service/)
add_subdirectory(ray_tracer/)
add_subdirectory(rt_algorithms/)
add_subdirectory(graphics_view/)

#add_library(rtc)
#target_link_libraries(rtc PUBLIC rtc_math input_methods data_model kd_tree)
