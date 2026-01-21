#include <algorithm>
#include <cmath>
#include <numeric>

#include "bench/stats.hpp"

using namespace bench::internal;

static double percentile(const std::vector<double>& latencies, double p) {
    double rank = (p / 100.0) * (latencies.size() - 1);
    size_t index = static_cast<size_t>(std::ceil(rank));
    return latencies[index];
}

Stats bench::internal::compute_stats(std::vector<double> latencies) {
    double N = static_cast<double>(latencies.size());

    std::sort(latencies.begin(), latencies.end());

    double min = latencies.front();
    double max = latencies.back();

    double sum = std::accumulate(latencies.begin(), latencies.end(), 0.0);
    double mean = sum / N;

    double sum_sq = 0.0;

    for (double l : latencies) {
        double diff = l - mean;
        sum_sq += diff * diff;
    }

    double var = sum_sq / N;
    double stddev = std::sqrt(var);

    return Stats {
        .min = min,
        .max = max,
        .p50   = percentile(latencies, 50.0),
        .p95   = percentile(latencies, 95.0),
        .p99   = percentile(latencies, 99.0),
        .p99_9 = percentile(latencies, 99.9),
        .stddev = stddev,
        .mean = mean
    };
}