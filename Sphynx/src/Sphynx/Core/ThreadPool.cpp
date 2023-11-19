#include "pch.h"
#include "ThreadPool.h"
#include "Application.h"

void Sphynx::Core::ThreadPool::Start(int numthreads)
{
    MaxThreads = numthreads;
    HasStarted.store(true);
    for (int i = 0; i < MaxThreads; i++) {
        threads.push_back(std::thread(&ThreadPool::Loop));
    }
    IsRunning.store(true);
}

void Sphynx::Core::ThreadPool::Submit(const std::function<void()>& new_job)
{
    {
        std::unique_lock<std::mutex> ul(ThreadPoolMutex);
        funcQueue.push_back(new_job);
    }
    condition.notify_one();
}

void Sphynx::Core::ThreadPool::SetStartUpCallback(const std::function<void()>& func)
{
    if(!HasStarted)
        startQueue.push_back(func);
}

void Sphynx::Core::ThreadPool::Loop()
{
    //Sphynx::Events::GlobalEventSystem::GetInstance()->QueueEvent()
    for (auto& func : startQueue) {
        func();
    }
    while (IsRunning) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> ul(ThreadPoolMutex);
            condition.wait(ul, [&]() {return !funcQueue.empty(); });
            job = funcQueue.front();
            funcQueue.pop_front();
        }
        job();
    }
}

void Sphynx::Core::ThreadPool::JoinAll()
{
    for (auto& thread : threads) {
        thread.join();
    }
}

void Sphynx::Core::ThreadPool::Stop()
{
    IsRunning.store(false);
    condition.notify_all();
}

void Sphynx::Core::ThreadPool::Abort()
{
    if (stopped) {

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
}
