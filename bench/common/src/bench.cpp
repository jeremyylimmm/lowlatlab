#include <format>

#include "bench/bench.hpp"
#include "bench/stats.hpp"

template<typename T>
static std::string format_time(T ns) {
    if (ns >= 1000000000) {
        return std::format("{}s", (ns/1000000000));
    }

    if (ns >= 1000000) {
        return std::format("{}ms", (ns/1000000));
    }

    if (ns >= 1000) {
        return std::format("{}µs", (ns/1000));
    }

    return std::format("{}ns", ns);
}

void bench::log_result(std::ostream& stream, const Result& result) {
    stream << "Benchmark: " << result.name << " | Iterations: " << result.iterations << std::endl;
    stream << "----------------------------------------------------------------------------" << std::endl;
    stream << "min: "       << format_time(result.min_ns)    << std::endl;
    stream << "max: "       << format_time(result.max_ns)    << std::endl;
    stream << "mean: "      << format_time(result.mean_ns)   << std::endl;
    stream << "stddev: "    << format_time(result.stddev_ns) << std::endl;
    stream << "p50: "       << format_time(result.p50_ns)    << std::endl;
    stream << "p95: "       << format_time(result.p95_ns)    << std::endl;
    stream << "p99: "       << format_time(result.p99_ns)    << std::endl;
    stream << "p99.9: "     << format_time(result.p99_9_ns)  << std::endl;
    stream << std::endl;
}

void bench::log_result_compact(std::ostream& stream, const Result& result) {
    stream << result.name << " | iters=" << result.iterations <<
    " | min="  << format_time(result.min_ns)    <<
    "max="     << format_time(result.max_ns)    <<
    "mean="    << format_time(result.mean_ns)   <<
    "stddev="  << format_time(result.stddev_ns) <<
    "p50="     << format_time(result.p50_ns)    <<
    "p95="     << format_time(result.p95_ns)     <<
    "p99="     << format_time(result.p99_ns)     <<
    "p99.9="   << format_time(result.p99_9_ns)   <<
    std::endl;
}