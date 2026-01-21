# lowlatlab

`lowlatlab` is a collection of low-latency systems experiments written in modern C++ (C++20), focused on understanding **concurrency**, **synchronization**, and **performance tradeoffs** at a practical level.

The goal of this repository is not to provide a production-ready library, but to build and benchmark core primitives from first principles, with an emphasis on **measurement, correctness, and interpretation**.


## Motivation

High-performance systems often fail not because of algorithms, but because of:
- hidden synchronization costs
- scheduler interaction
- tail latency
- poorly understood benchmarks

This project exists to explore those issues directly by:
- implementing fundamental data structures
- measuring them under realistic conditions
- explaining *why* they behave the way they do


## Contents

### 📦 Queues

Two queue implementations used to study synchronization tradeoffs:

- **Mutex Queue**
  - General-purpose, `std::mutex`-protected queue
  - Simple semantics
  - Sensitive to contention and scheduling

- **SPSC Queue**
  - Lock-free single-producer single-consumer ring buffer
  - Uses atomic indices with acquire/release semantics
  - Designed for predictable latency and bounded tails

Each implementation includes:
- correctness tests
- detailed benchmarks
- analysis of results and tradeoffs

➡️ See: [`queue/README.md`](queue/README.md)


### 🧪 Benchmarks

A small benchmarking harness used across the project to:
- measure latency distributions (p50–p99.9)
- detect tail behavior
- compare implementations under identical workloads

Key features:
- nanosecond-resolution timing
- percentile and variance reporting
- configurable workload size and contention model
- support for thread pinning and busy-spin workloads

Benchmarks are intentionally separated from correctness tests.

### ✅ Tests

Unit and stress tests built with Catch2:
- correctness under concurrency
- move-only type support
- absence of lost or duplicated data
- termination safety

Tests focus on **correctness**, not performance.

## Key Learnings So Far

Some conclusions supported by real measurements:

- Lock-free does not always mean faster
- Mutexes can outperform atomics in uncontended steady-state workloads
- Lock-free queues shine in:
  - tail latency
  - predictability
  - avoiding scheduler interaction
- Benchmark structure matters more than micro-optimizations
- OS scheduling dominates end-to-end latency unless explicitly controlled

Each experiment documents both results and interpretation.


## Build

The project uses CMake and targets Linux.

```bash
mkdir build
cd build
cmake ..
make
```

## Why This Exists

This repository is intended as:

- a learning exercise in low-level performance

- a reference for common concurrency pitfalls

- a portfolio project demonstrating systems thinking

The emphasis is on understanding behavior, not just writing code that “looks fast”.