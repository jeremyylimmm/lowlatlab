#include "bench.hpp"
#include "stats.hpp"
#include "timer.hpp"

using namespace bench;
using namespace internal;

template<typename Fn>
Result run(const std::string& name, size_t iterations, Fn&& fn) {
    std::vector<uint64_t> latencies(iterations);

    for (size_t i = 0; i < iterations; ++i) {
        uint64_t start = now_ns();

        fn();

        uint64_t end = now_ns();

        
    }

    Stats stats = compute_stats(latencies);
}