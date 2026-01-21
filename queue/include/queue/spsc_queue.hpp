#pragma once

#include <cstdint>
#include <atomic>
#include <vector>
#include <optional>

template<typename T, std::size_t C = 256>
class SPSCQueue {
public:
    SPSCQueue()
        : _buffer(C), _head(0), _tail(0)
    {}

    template<typename U>
    bool enqueue(U&& value) {
        const auto tail = _tail.load(std::memory_order_relaxed);
        const auto next_tail = increment(tail);

        if (next_tail == _head.load(std::memory_order_acquire)) { // keep one element empty otherwise full-queue == empty-queue
            return false;
        }

        _buffer[tail] = std::move(value);

        _tail.store(next_tail, std::memory_order_release);
        return true;
    }

    std::optional<T> dequeue() {
        const auto head = _head.load(std::memory_order_relaxed);

        if (head == _tail.load(std::memory_order_acquire)) {
            return std::nullopt;
        }

        T value = std::move(_buffer[head]);

        _head.store(increment(head), std::memory_order_release); 

        return value;
    }

    bool empty() {
        const auto head = _head.load(std::memory_order_relaxed); // no touching memory so relaxed is okay
        const auto tail = _tail.load(std::memory_order_relaxed);
        return head == tail;
    }

private:
    size_t increment(size_t x) {
        return (x + 1) % C;
    }

private:
    std::vector<T> _buffer;
    std::atomic<size_t> _head;
    std::atomic<size_t> _tail;
};