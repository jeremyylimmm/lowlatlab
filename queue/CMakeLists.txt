file(GLOB_RECURSE SOURCE src/*.cpp)

add_library(queue ${SOURCE})
target_compile_options(queue PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(queue PRIVATE include)