add_subdirectory(common)

add_executable(bench_queues bench_queues.cpp)
target_link_libraries(bench_queues PRIVATE bench)
target_include_directories(bench_queues PRIVATE common/include)
target_include_directories(bench_queues PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../queue/include)