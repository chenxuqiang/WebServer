#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <unistd.h>

struct Task {
    void (*func)(void*);
    void *args;
};

class ThreadPool {
public:
    ThreadPool(size_t);
    ~ThreadPool();
    void EnQueue(Task *task);
private:
    std::vector<std::thread> workers;
    std::queue<Task*> tasks;

    std::mutex queueMutex;
    std::condition_variable cond;
    bool stop;
};

inline ThreadPool::ThreadPool(size_t size): stop(false)
{
    for (size_t i = 0; i < size; i++) {
        workers.emplace_back(
            [this, i]
            {
                while (true) {
                    Task *task = NULL;
                    {
                        std::unique_lock<std::mutex> lock(this->queueMutex);
                        this->cond.wait(lock, [this]{ return this->stop || !this->tasks.empty(); });
                        if (this->stop && this->tasks.empty()) {
                            return ;
                        }

                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }

                    std::cout << "thread index = " << i << std::endl;
                    task->func(task->args);
                    sleep(4);
                }
            }
        );
    }
}

void ThreadPool::EnQueue(Task *task)
{
    std::unique_lock<std::mutex> lock(queueMutex);

    if (stop) {
        throw std::runtime_error("enqueue on stopped ThreadPool");
    }

    tasks.emplace(task);
    cond.notify_one();
}

inline ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }

    cond.notify_all();
    for (std::thread &worker : workers) {
        worker.join();
    }
}

#endif