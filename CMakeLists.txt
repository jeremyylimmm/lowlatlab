cmake_minimum_required(VERSION 3.20)

project(lowlatlab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_subdirectory(bench)
add_subdirectory(queue)

find_package(Catch2 3 REQUIRED)

add_executable(test_mutex_queue tests/test_mutex_queue.cpp)
target_include_directories(test_mutex_queue PRIVATE queue/include third_party/Catch2/src)
target_link_libraries(test_mutex_queue PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(test_mutex_queue)
