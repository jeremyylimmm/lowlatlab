#pragma once

#include <queue>
#include <mutex>
#include <optional>

template<typename T>
class MutexQueue {
public:
    MutexQueue() = default;
    ~MutexQueue() = default;

    void enqueue(T item) {
        std::lock_guard lock(_mutex);
        _queue.push(std::move(item));
    }

    std::optional<T> dequeue() {
        std::lock_guard lock(_mutex);

        if (_queue.empty()) {
            return std::nullopt;
        }

        T out = std::move(_queue.front());
        _queue.pop();

        return out;
    }

    bool empty() {
        std::lock_guard lock(_mutex);
        return _queue.empty();
    }

    size_t size() {
        std::lock_guard lock(_mutex);
        return _queue.size();
    }

private:
    std::queue<T> _queue;
    std::mutex _mutex;
};