#include <iostream>

#include <bench/bench.hpp>

using namespace bench;

int main() {
    volatile int x = 0;

    Result result = run("increment", 100000, [&]{
        x += 1;
    });

    log_result(std::cout, result);
    log_result_compact(std::cout, result);

    return x;
}