#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

#ifndef CONCURRENTQUEUE_H
#define CONCURRENTQUEUE_H

template <typename T>
class ConcurrentPriorityQueue {
private:
    std::priority_queue<T, std::vector<T>, std::function<bool(T, T)>> queue;
    std::mutex mutex;
    std::condition_variable cond;

public:
    ConcurrentPriorityQueue(const std::function<bool(T, T)>& comp)
        : queue(comp) {}

    void push(const T& value) {
        {
            std::unique_lock<std::mutex> lock(this->mutex);
            queue.push(value);
        }
        this->cond.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(this->mutex);
        while (queue.empty()) {
            this->cond.wait(lock);
        }
        T value = queue.top();
        queue.pop();
        return value;
    }
};

#endif