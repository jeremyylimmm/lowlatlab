#include <algorithm>
#include <cmath>

#include "bench/stats.hpp"

using namespace bench::internal;

static uint64_t percentile(const std::vector<uint64_t>& latencies, double p) {
    double rank = (p / 100.0) * (latencies.size() - 1);
    size_t index = static_cast<size_t>(std::ceil(rank));
    return latencies[index];
}

Stats bench::internal::compute_stats(std::vector<uint64_t> latencies) {
    std::sort(latencies.begin(), latencies.end());

    uint64_t min = UINT64_MAX;
    uint64_t max = 0;
    double sum = 0;

    for (uint64_t l : latencies) {
        min = std::min(min, l);
        max = std::max(max, l);
        sum += static_cast<double>(l);
    }

    double mean = sum / static_cast<double>(latencies.size());

    return Stats {
        .min = min,
        .max = max,
        .p50   = percentile(latencies, 50.0),
        .p95   = percentile(latencies, 95.0),
        .p99   = percentile(latencies, 99.0),
        .p99_9 = percentile(latencies, 99.9),
        .mean = mean
    };
}