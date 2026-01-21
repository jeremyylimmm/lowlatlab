# Queue Implementations

This directory contains two queue implementations used to explore concurrency, synchronization, and performance tradeoffs in C++:

- `mutex_queue` — a queue protected by a `std::mutex`
- `spsc_queue` — a lock-free single-producer single-consumer (SPSC) ring buffer

Both queues expose a non-blocking API (`enqueue` / `dequeue`) and are benchmarked under identical workloads to compare behavior under realistic conditions.

## Mutex Queue

The mutex queue is a general-purpose thread-safe queue built on top of a standard container and protected by a `std::mutex`.

**Characteristics:**
- Supports arbitrary producer/consumer patterns
- Simple and easy to reason about
- May block or incur kernel involvement under contention
- Performance depends heavily on scheduling and contention patterns

This implementation serves as a baseline and a comparison point for the lock-free design.

## SPSC Queue

The SPSC queue is a lock-free ring buffer designed for exactly one producer thread and one consumer thread.

**Characteristics:**
- No locks, no blocking
- Uses atomic indices with acquire/release semantics
- Requires fixed capacity
- Guarantees FIFO order
- Avoids kernel involvement entirely

The design prioritizes:
- predictable latency
- bounded tail behavior
- suitability for low-latency pipelines

This queue is representative of structures commonly used in high-performance systems (e.g. trading, telemetry, real-time pipelines).

## Benchmarks

The queues are benchmarked using an external benchmarking harness to compare **steady-state streaming performance**.

### Benchmark setup

- **Model:** Single producer, single consumer
- **Threads:** Pinned to separate CPU cores
- **Scheduling:** Busy-spin (no sleeping or blocking)
- **Batch size:** `N = 10,240` items per iteration
- **Iterations:** 10,000
- **Measurement:** Total enqueue + dequeue time per batch
- **Derived metric:** Per-item latency (`total_time / N`)

Each iteration measures the time to enqueue and dequeue an entire batch, amortizing startup and scheduling effects.

## Results

### Mutex queue

```
mean: 2.10 ms (~205 ns / item)
p50: 2.09 ms
p99: 2.77 ms
p99.9: 3.09 ms
stddev: 250 µs
```

### SPSC queue (capacity = 256)

```
mean: 3.31 ms (~323 ns / item)
p50: 3.31 ms
p99: 3.75 ms
p99.9: 4.31 ms
stddev: 122 µs
```

## Interpretation

These benchmarks measure **steady-state streaming throughput**, not contention.

In this regime:

- The mutex is uncontended and remains on the fast user-space path
- Lock/unlock overhead is amortized across a tight loop
- The SPSC queue performs atomic operations and cross-core cache-line transfers on every operation

As a result, the mutex queue achieves **lower mean latency** in this specific scenario.

However, the SPSC queue exhibits:

- Lower variance
- Tighter latency distribution
- No blocking or kernel involvement

This makes it more suitable for **latency-sensitive systems**, where predictability and tail latency matter more than peak throughput.

## Key takeaway

> Lock-free queues are not universally faster.  
> They trade peak throughput for predictability, bounded latency, and freedom from scheduler interaction.

These results demonstrate why high-performance systems often favor SPSC queues in hot paths, even when mutex-based designs can outperform them in uncontended steady-state benchmarks.

## Notes

- Different workloads (smaller batches, contention, blocking producers) strongly favor the SPSC design.
- The benchmark intentionally avoids sleeps or condition variables to expose raw synchronization costs.
- Results will vary by CPU, kernel, and power-management configuration.