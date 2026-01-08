file(GLOB_RECURSE SOURCE src/*.cpp)

add_library(bench ${SOURCE})
target_compile_options(bench PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(bench PRIVATE include)

add_executable(bench_empty examples/empty.cpp)
target_link_libraries(bench_empty PRIVATE bench)
target_include_directories(bench_empty PRIVATE include)