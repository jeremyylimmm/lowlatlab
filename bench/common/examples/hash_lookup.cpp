#include <iostream>
#include <unordered_map>

#include <bench/bench.hpp>

using namespace bench;

int main() {
    std::unordered_map<int, int> map = {{1, 10}, {2, 20}, {3, 30}};
    volatile int sink;

    Result result = run("empty", 100000, [&]{
        sink = map[2];
    });
    
    log_result(std::cout, result);
    log_result_compact(std::cout, result);

    return 0;
}