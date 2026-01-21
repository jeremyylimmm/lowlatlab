#include <iostream>

#include <bench/bench.hpp>

using namespace bench;

int main() {
    volatile int* sink;

    Result result = run("heap_alloc", 100000, [&]{
        sink = new int[10];
        delete[] sink;
    });

    log_result(std::cout, result);
    log_result_compact(std::cout, result);

    return 0;
}