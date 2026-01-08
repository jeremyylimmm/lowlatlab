file(GLOB_RECURSE SOURCE src/*.cpp)

add_library(bench ${SOURCE})

target_compile_options(bench PRIVATE -Wall -Wextra -Wpedantic)

target_include_directories(bench PRIVATE include/bench)