#pragma once
#include <mutex>
#include <thread>
#include <atomic>
#include <queue>
#include <future>

namespace Sphynx {
	class Application;
}

namespace Sphynx::Core {
	class ThreadPool
	{
	private:
		int MaxThreads;
		std::vector<std::thread> threads;
		std::deque<std::function<void()>> funcQueue;
		std::condition_variable condition;
		std::mutex ThreadPoolMutex;
		std::atomic<bool> IsRunning{ false };
		std::atomic<bool> FinishedWork{ true };
		bool stopped = false;
		ThreadPool operator=(const ThreadPool& tp) = delete;
		ThreadPool(const ThreadPool&) = delete;
		void Loop();
		void JoinAll();
		void Abort();
	public:
		ThreadPool(int num_threads = std::thread::hardware_concurrency());
		~ThreadPool();
		void Stop();
		void Start(Application* app);

		//Delegates ?
		void Submit(std::function<void()> new_job);
		int GetMaxThreads() { return MaxThreads; };
	};
}
