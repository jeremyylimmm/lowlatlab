#pragma once

#include <string>
#include <cstdint>
#include <iostream>
#include <vector>

#include "bench/timer.hpp"
#include "bench/stats.hpp"

namespace bench {

    struct Result {
        uint64_t min_ns;
        uint64_t max_ns;
        double   mean_ns;
        uint64_t p50_ns;
        uint64_t p95_ns;
        uint64_t p99_ns;
        uint64_t p99_9_ns;

        double stddev_ns;

        size_t iterations;
        std::string name;

        std::vector<uint64_t> latencies;
    };

    template<typename Fn>
    Result run(const std::string& name, size_t iterations, Fn&& fn) {
        std::vector<uint64_t> latencies(iterations);

        size_t warmup_iterations = iterations / 5;

        for (size_t i = 0; i < warmup_iterations; ++i) {
            fn();
        }

        for (size_t i = 0; i < iterations; ++i) {
            uint64_t start = internal::now_ns();

            fn();

            uint64_t end = internal::now_ns();

            latencies[i] = end-start;
        }

        internal::Stats stats = internal::compute_stats(latencies);

        return Result {
            .min_ns = stats.min,
            .max_ns = stats.max,
            .mean_ns = stats.mean,
            .p50_ns = stats.p50,
            .p95_ns = stats.p95,
            .p99_ns = stats.p99,
            .p99_9_ns = stats.p99_9,
            .stddev_ns = stats.stddev,
            .iterations = iterations,
            .name = name,
            .latencies = latencies,
        };
    }

    void log_result(std::ostream& stream, const Result& result);
    void log_result_compact(std::ostream& stream, const Result& result);
}