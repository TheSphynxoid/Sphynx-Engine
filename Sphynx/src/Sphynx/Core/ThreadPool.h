#pragma once
#include <mutex>
#include <thread>
#include <atomic>
#include <queue>
#include <future>
#include "Delegate.h"

namespace Sphynx {
	class Application;
}

namespace Sphynx::Core {
	class ThreadPool
	{
	private:
		inline thread_local static bool isMainThread = false;
		inline static int MaxThreads;
		inline static std::vector<std::thread> threads;
		inline static std::deque<std::function<void()>> funcQueue;
		inline static std::condition_variable condition;
		inline static std::mutex ThreadPoolMutex;
		inline static std::atomic<bool> IsRunning{ false };
		inline static std::atomic<bool> FinishedWork{ true };
		inline static std::atomic<bool> HasStarted{ false };
		inline static std::vector<std::function<void()>> startQueue;
		inline thread_local static std::deque<std::function<void()>> ThreadLocalJobs;
		inline static bool stopped = false;
		ThreadPool() = delete;
		ThreadPool(const ThreadPool&) = delete;
		ThreadPool operator=(const ThreadPool&) = delete;
		ThreadPool(ThreadPool&&) = delete;
		ThreadPool operator=(ThreadPool&&) = delete;
		static void Loop();
		static void JoinAll();
		static void Abort();
	public:
		static void Start(int num_threads = std::thread::hardware_concurrency());
		static void Stop();

		//Pushes Job to queue.
		static void PushJobToQueue(const std::function<void()>& new_job) noexcept {
			ThreadLocalJobs.push_back(new_job);
		}

		//Will block until able to push jobs or times out.
		static void PushQueueToWorker() {
			if (!ThreadLocalJobs.empty())  {
				//Scope
				{
					std::unique_lock<std::mutex> ul = std::unique_lock<std::mutex>(ThreadPoolMutex);
					funcQueue.insert(funcQueue.cend(), ThreadLocalJobs.cbegin(), ThreadLocalJobs.cend());
				}
				//End of Scope
				//Serial jobs on the other thread i think, unless another one wakes up
				condition.notify_one();
				ThreadLocalJobs.clear();
			}
		}
		
		//Needs testing.
		//if your function has an error it's better to raise an exception as this will catch it and the value will be set to the exception.
		template<typename T>
		static std::promise<T> Submit(std::function<T()>& new_job) noexcept{
			std::promise<T> promise;
			[&new_job, &promise]() 
			{
				try {
					promise.set_value(new_job());
				}
				catch (...) {
					promise.set_exception(std::current_exception());
				}
			};
			return promise;
		}

		static void Submit(const std::function<void()>& new_job);
		template<class T>
		static void Submit(Delegate<void, T, void>& del) {
			{
				Promise(std::abort);
				std::unique_lock<std::mutex> ul(ThreadPoolMutex);
				this->funcQueue.push_back([&del]()->void { del(); });
			}
			condition.notify_one();
		}
		static const int GetMaxThreads() noexcept { return MaxThreads; };
		static std::vector<std::thread>& GetThreads() noexcept { return threads; };
		static std::unique_lock<std::mutex>&& GetLock() noexcept { return std::unique_lock<std::mutex>(ThreadPoolMutex); };
		static bool IsMainThread()noexcept { return isMainThread; }
		static void SetStartUpCallback(const std::function<void()>& func);
	};
}
