#pragma once

#include <queue>
#include <mutex>

template<typename T>
class MutexQueue {
public:
    MutexQueue() = default;
    ~MutexQueue() = default;

    void enqueue(T&& item) {
        std::lock_guard lock(_mutex);
        _queue.push(std::forward(item));
    }

    bool dequeue(T& out) {
        std::lock_guard lock(_mutex);
        out = _queue.front();
        _queue.pop();
    }

    size_t size() const {
        std::lock_guard lock(_mutex);
        return _queue.size();
    }

private:
    std::queue<T> _queue;
    std::mutex _mutex;
};