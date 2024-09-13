// ThreadSafeQueue.h
#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class ThreadSafeQueue {
public:
    void push(const T& value) {
        std::lock_guard<std::mutex> lock(mtx);
        q.push(value);
        cv.notify_one();
    }

    bool tryPop(T& value) {
        std::lock_guard<std::mutex> lock(mtx);
        if (q.empty()) return false;
        value = q.front();
        q.pop();
        return true;
    }

    void waitAndPop(T& value) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]{ return !q.empty(); });
        value = q.front();
        q.pop();
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx);
        return q.empty();
    }

private:
    mutable std::mutex mtx;
    std::queue<T> q;
    std::condition_variable cv;
};

#endif // THREADSAFEQUEUE_H
