#include <ctime>

#include "bench/timer.hpp"

using namespace bench::internal;

uint64_t now_ns() {
    timespec t;
    clock_gettime(CLOCK_MONOTONIC_RAW, &t);
    return t.tv_nsec;
}