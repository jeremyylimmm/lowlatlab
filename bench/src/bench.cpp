#include "bench/bench.hpp"
#include "bench/stats.hpp"

void bench::log_result(std::ostream& stream, const Result& result) {
    stream << "Benchmark: " << result.name << " | Iterations: " << result.iterations << std::endl;
    stream << "----------------------------------------------------------------------------" << std::endl;
    stream << "min: "   << result.min_ns   << "ns" << std::endl;
    stream << "max: "   << result.max_ns   << "ns" << std::endl;
    stream << "mean: "  << result.mean_ns  << "ns" << std::endl;
    stream << "stddev: "  << result.stddev_ns  << "ns" << std::endl;
    stream << "p50: "   << result.p50_ns   << "ns" << std::endl;
    stream << "p95: "   << result.p95_ns   << "ns" << std::endl;
    stream << "p99: "   << result.p99_ns   << "ns" << std::endl;
    stream << "p99.9: " << result.p99_9_ns << "ns" << std::endl;
}

void bench::log_result_compact(std::ostream& stream, const Result& result) {
    stream << result.name << " | iters=" << result.iterations <<
    " | min="  << result.min_ns   << "ns " <<
    "max="  << result.max_ns      << "ns " <<
    "mean=" << result.mean_ns     << "ns " <<
    "stddev=" << result.stddev_ns << "ns " <<
    "p50="     << result.p50_ns   << "ns " <<
    "p95="     << result.p95_ns   << "ns " <<
    "p99="     << result.p99_ns   << "ns " <<
    "p99.9="   << result.p99_9_ns << "ns " <<
    std::endl;
}