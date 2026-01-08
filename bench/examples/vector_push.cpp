#include <iostream>

#include <bench/bench.hpp>

using namespace bench;

int main() {
    std::vector<int> v;

    Result result = run("vector_push", 100000, [&]{
        v.push_back(1);

        if (v.size() > 256) {
            v.clear();
        }
    });

    log_result(std::cout, result);
    log_result_compact(std::cout, result);

    return 0;
}