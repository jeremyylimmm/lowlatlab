#include <iostream>
#include <thread>
#include <format>

#include <bench/bench.hpp>
#include <queue/spsc_queue.hpp>
#include <queue/mutex_queue.hpp>

#include <pthread.h>

inline void pin_thread_to_core(int core_id) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);

    int rc = pthread_setaffinity_np(
        pthread_self(),
        sizeof(cpu_set_t),
        &cpuset
    );

    if (rc != 0) {
        throw std::runtime_error("Failed to set thread affinity");
    }
}

template<std::size_t C>
static bench::Result spsc_queue_benchmark(int N, int iterations) {
    return bench::run(std::format("spsc_queue (N={}, C={})", N, C), iterations, [N]{
        SPSCQueue<int, C> q;

        std::thread producer([&q, N](){
            pin_thread_to_core(2);

            for (int i = 0; i < N; ++i) {
                while (!q.enqueue(i)) {
                }
            }
        });

        int consumed = 0;

        std::thread consumer([&q, N, &consumed](){
            pin_thread_to_core(4);

            while (consumed < N) {
                auto item = q.dequeue();

                if (!item.has_value()) {
                    continue;
                }

                consumed++;
            }
        });

        producer.join();
        consumer.join();
    });
}

int main() {
    int N = 1024*10;
    int iterations = 10000;

    bench::Result result = bench::run(std::format("mutex_queue (N={})", N), iterations, [N]{
        MutexQueue<int> q;

        std::thread producer([&q, N](){
            pin_thread_to_core(2);

            for (int i = 0; i < N; ++i) {
                q.enqueue(i);
            }
        });

        int consumed = 0;

        std::thread consumer([&q, N, &consumed](){
            pin_thread_to_core(4);

            while (consumed < N) {
                auto item = q.dequeue();

                if (!item.has_value()) {
                    continue;
                }

                consumed++;
            }
        });

        producer.join();
        consumer.join();
    });

    log_result(std::cout, result);

    result = spsc_queue_benchmark<256>(N, iterations);
    log_result(std::cout, result);

    return 0;
}