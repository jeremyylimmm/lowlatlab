#include <iostream>

#include <bench/bench.hpp>

using namespace bench;

int main() {
    Result result = run("empty", 100000, []{});
    log_result(std::cout, result);
    log_result_compact(std::cout, result);
}