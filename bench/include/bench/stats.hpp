#pragma once

#include <cstdint>
#include <vector>

namespace bench { namespace internal {

    struct Stats {
        uint64_t min;
        uint64_t max;
        uint64_t p50;
        uint64_t p95;
        uint64_t p99;
        uint64_t p99_9;
        double mean;
    };

    Stats compute_stats(std::vector<uint64_t> latencies);

}}