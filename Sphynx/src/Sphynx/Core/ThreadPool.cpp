#include "pch.h"
#include "ThreadPool.h"
#include "Application.h"

Sphynx::Core::ThreadPool::ThreadPool(int num_threads)
{
    if (!num_threads) {
        //Without the thread pool nothing will work.
        throw std::invalid_argument("Thread Number must be non-zero");
    }
    MaxThreads = num_threads;
}

Sphynx::Core::ThreadPool::~ThreadPool()
{
    if (!stopped) {
        Abort();
    }
}

void Sphynx::Core::ThreadPool::Start(Application* app)
{
    for (int i = 0; i < MaxThreads; i++) {
        threads.push_back(std::thread(&ThreadPool::Loop, this));
    }
}

void Sphynx::Core::ThreadPool::Submit(std::function<void()> new_job)
{
    this->funcQueue.push_back(new_job);
    condition.notify_one();
}

void Sphynx::Core::ThreadPool::Loop()
{
    std::unique_lock<std::mutex> ul(ThreadPoolMutex);
    while (IsRunning || (!FinishedWork && !funcQueue.empty())) {
        if (!funcQueue.empty()) {
            std::function<void()> job(std::move(funcQueue.front()));
            funcQueue.pop_front();
            ul.unlock();
            job();
            ul.lock();
        }
        else {
            condition.wait(ul);
        }
    }
}

void Sphynx::Core::ThreadPool::JoinAll()
{
}

void Sphynx::Core::ThreadPool::Stop()
{
    IsRunning.store(false);
    condition.notify_all();
}

void Sphynx::Core::ThreadPool::Abort()
{
    {
        std::unique_lock<std::mutex> lock(ThreadPoolMutex);
        IsRunning.store(false); // use this flag in condition.wait
    }
    condition.notify_all(); // wake up all threads.

    // Join all threads.
    for (std::thread& every_thread : threads)
    {
        every_thread.join();
    }

    threads.clear();
    stopped = true; // use this flag in destructor, if not set, call shutdown() 
}
