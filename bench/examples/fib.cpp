#include <iostream>

#include <bench/bench.hpp>

using namespace bench;

int main() {
    auto fib = [](int n, auto&& self) -> int {
        return n < 2 ? n : self(n-1, self) + self(n-2, self);
    };

    volatile int sink;

    Result result = run("fib", 100000, [&]{
        sink = fib(5, fib);
    });

    log_result(std::cout, result);
    log_result_compact(std::cout, result);

    return 0;
}