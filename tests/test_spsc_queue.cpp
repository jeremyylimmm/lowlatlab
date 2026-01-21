#include <memory>
#include <thread>
#include <vector>
#include <set>

#include <queue/spsc_queue.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Dequeue on empty queue", "SPSC Queue") {
    SPSCQueue<int> q;
    REQUIRE(q.dequeue().has_value() == false);
}

TEST_CASE("Empty queue reports empty", "SPSC Queue") {
    SPSCQueue<int> q;
    REQUIRE(q.empty());
}

TEST_CASE("Single item queue/dequeue", "SPSC Queue") {
    SPSCQueue<int, 256> q;

    int x = 34;
    REQUIRE(q.enqueue(x) == true);

    auto item = q.dequeue();

    REQUIRE(item.has_value() == true);
    REQUIRE(*item == x);
}

TEST_CASE("Multiple items maintain integrity", "SPSC Queue") {
    SPSCQueue<int, 256> q;

    size_t n = 10;

    for (size_t i = 0; i < n; ++i) {
        REQUIRE(q.enqueue(i) == true);
    }

    for (size_t i = 0; i < n; ++i) {
        auto item = q.dequeue();
        REQUIRE(item.has_value() == true);
        REQUIRE(*item == i);
    }

    REQUIRE(q.dequeue().has_value() == false);
}

TEST_CASE("Move-only types work", "SPSC Queue") {
    SPSCQueue<std::unique_ptr<int>> q;

    size_t n = 10;

    for (size_t i = 0; i < n; ++i) {
        auto x = std::make_unique<int>(i);
        REQUIRE(q.enqueue(std::move(x)) == true);
    }

    for (size_t i = 0; i < n; ++i) {
        auto item = q.dequeue();
        REQUIRE(item.has_value() == true);
        REQUIRE(**item == i);
    }
}

TEST_CASE("Single producer and single consumer operate simultaenously", "SPSC Queue") {
    SPSCQueue<int, 256> q;

    int N = 1024*1024;

    std::vector<int> produced_values;
    produced_values.reserve(N);

    std::thread producer([&q, N, &produced_values]()
    {
        for (int i = 0; i < N; ++i) {
            while(!q.enqueue(i)) {
                std::this_thread::yield();
            }

            produced_values.push_back(i);
        }
    });

    std::vector<int> consumed_values;
    consumed_values.reserve(N);

    std::thread consumer([&q, N, &consumed_values]()
    {
        while (consumed_values.size() < N) {
            auto item = q.dequeue();

            if (!item.has_value()) {
                std::this_thread::yield();
                continue;
            }

            consumed_values.push_back(*item);
        }
    });

    producer.join();
    consumer.join();

    std::multiset produced_set(produced_values.begin(), produced_values.end());
    std::multiset consumed_set(consumed_values.begin(), consumed_values.end());

    REQUIRE(produced_set.size() == N);
    REQUIRE(consumed_set.size() == N);

    REQUIRE(q.empty());
    REQUIRE(produced_set == consumed_set);
}