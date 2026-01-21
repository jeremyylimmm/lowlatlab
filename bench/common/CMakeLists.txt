file(GLOB_RECURSE SOURCE src/*.cpp)

add_library(bench ${SOURCE})
target_compile_options(bench PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(bench PRIVATE include)

add_executable(bench_empty examples/empty.cpp)
target_link_libraries(bench_empty PRIVATE bench)
target_include_directories(bench_empty PRIVATE include)

add_executable(bench_increment examples/increment.cpp)
target_link_libraries(bench_increment PRIVATE bench)
target_include_directories(bench_increment PRIVATE include)

add_executable(bench_heap_alloc examples/heap_alloc.cpp)
target_link_libraries(bench_heap_alloc PRIVATE bench)
target_include_directories(bench_heap_alloc PRIVATE include)

add_executable(bench_vector_push examples/vector_push.cpp)
target_link_libraries(bench_vector_push PRIVATE bench)
target_include_directories(bench_vector_push PRIVATE include)

add_executable(bench_fib examples/fib.cpp)
target_link_libraries(bench_fib PRIVATE bench)
target_include_directories(bench_fib PRIVATE include)

add_executable(bench_hash_lookup examples/hash_lookup.cpp)
target_link_libraries(bench_hash_lookup PRIVATE bench)
target_include_directories(bench_hash_lookup PRIVATE include)