#pragma once
// 세마포어 헤더 파일
#include <mutex>
#include <condition_variable>

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

class SemaPhore {
private:
    std::mutex mtx;
    std::condition_variable cv;
    int count;

public:
    SemaPhore(int count_ = 0) : count(count_) { }

    void notify() {
        std::unique_lock<std::mutex> lock(mtx);
        ++count;
        cv.notify_one();
    }

    void wait() {
        std::unique_lock<std::mutex> lock(mtx);
        while (count == 0) {
            cv.wait(lock);
        }
        --count;
    }
};

#endif