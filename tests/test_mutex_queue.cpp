#include <memory>
#include <thread>
#include <vector>
#include <set>

#include <queue/mutex_queue.hpp>
#include <catch2/catch_test_macros.hpp>


TEST_CASE("Dequeue on empty queue", "Mutex Queue") {
    MutexQueue<int> q;
    REQUIRE(q.dequeue().has_value() == false);
}

TEST_CASE("Empty queue reports empty", "Mutex Queue") {
    MutexQueue<int> q;
    REQUIRE(q.empty());
}

TEST_CASE("Single item queue/dequeue", "Mutex Queue") {
    MutexQueue<int> q;

    int x = 34;
    q.enqueue(x);

    auto item = q.dequeue();

    REQUIRE(item.has_value() == true);
    REQUIRE(*item == x);
}

TEST_CASE("Multiple items maintain integrity", "Mutex Queue") {
    MutexQueue<int> q;

    size_t n = 10;

    for (size_t i = 0; i < n; ++i) {
        REQUIRE(q.size() == i);
        q.enqueue(i);
    }

    for (size_t i = 0; i < n; ++i) {
        auto item = q.dequeue();
        REQUIRE(item.has_value() == true);
        REQUIRE(*item == i);
    }

    REQUIRE(q.dequeue().has_value() == false);
}

TEST_CASE("Move-only types work", "Mutex Queue") {
    MutexQueue<std::unique_ptr<int>> q;

    size_t n = 10;

    for (size_t i = 0; i < n; ++i) {
        auto x = std::make_unique<int>(i);
        q.enqueue(std::move(x));
    }

    for (size_t i = 0; i < n; ++i) {
        auto item = q.dequeue();
        REQUIRE(item.has_value() == true);
        REQUIRE(**item == i);
    }
}

TEST_CASE("Queue is thread-safe", "Mutex Queue") {
    MutexQueue<int> q;

    int num_producers = 4;
    int num_consumers = 4;
    int items_per_producer = 10000;

    std::vector<std::thread> producers;
    std::vector<std::vector<int>> produced_values(num_producers);

    for (int p = 0; p < num_producers; ++p) {
        producers.emplace_back([&q, p, items_per_producer, &produced_values]()
        {
            auto& pv = produced_values[p];

            for (int i = 0; i < items_per_producer; ++i) {
                int value = p * items_per_producer + i;
                q.enqueue(value);
                pv.push_back(value);
            }
        });
    }

    std::vector<std::thread> consumers;
    std::vector<std::vector<int>> consumed_values(num_consumers);

    for (int c = 0; c < num_consumers; ++c) {
        consumers.emplace_back([&q, c, &consumed_values]()
        {
            auto& cv = consumed_values[c];

            for (;;) {
                auto item = q.dequeue();
                if (!item.has_value()) {
                    std::this_thread::yield();
                    if (q.empty()) {
                        break;
                    }
                    else {
                        continue;
                    }
                }
                cv.push_back(*item);
            }
        });
    }

    for (auto& t : producers) t.join();
    for (auto& t : consumers) t.join();

    std::multiset<int> all_produced;
    std::multiset<int> all_consumed;

    for (auto& pv : produced_values) {
        all_produced.insert(pv.begin(), pv.end());
    }

    for (auto& cv : consumed_values) {
        all_consumed.insert(cv.begin(), cv.end());
    }

    REQUIRE(all_produced.size() == num_producers * items_per_producer);
    REQUIRE(all_consumed.size() == num_producers * items_per_producer);

    REQUIRE(q.empty());
    REQUIRE(all_produced == all_consumed);
}