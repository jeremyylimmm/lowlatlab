#pragma once

#include <cstdint>
#include <vector>

namespace bench { namespace internal {

    struct Stats {
        double min;
        double max;
        double p50;
        double p95;
        double p99;
        double p99_9;
        double stddev;
        double mean;
    };

    Stats compute_stats(std::vector<double> latencies);

}}