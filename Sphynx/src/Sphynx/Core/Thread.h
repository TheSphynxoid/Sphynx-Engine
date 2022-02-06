#pragma once
#include <thread>
#include <atomic>
#include <mutex>

namespace Sphynx {
	class Thread
	{
	protected:
		std::thread thread;
	public:
		Thread() {
		}
		virtual ~Thread() {
			thread.join();
		}
		template<typename fty>
		Thread(std::function<fty> f) {
			thread = std::thread(f);
		}
		void Join() {
			thread.join();
		}
		bool Joinable() {
			return thread.joinable();
		}
		void Detach() {
			thread.detach();
		}
		std::thread::id GetID() {
			return thread.get_id();
		}
		template<typename fty>
		void SetJob(std::function<fty> func) {
			thread = std::thread(func);
		}
	};
	class LoopThread : public Thread {
	private:
		std::atomic<bool> IsRunning = false;
		std::deque<std::function<void()>> funcQueue;
		std::condition_variable condition;
		std::mutex ThreadMutex;
		bool Stopped;
		void Loop() {
			std::unique_lock<std::mutex> ul(ThreadMutex);
			while (IsRunning || !funcQueue.empty()) {
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
	public:
		LoopThread() {
			thread = std::thread(&LoopThread::Loop, this);
			IsRunning.store(true);
		}

		~LoopThread() {
			Abort();
		}
		void Abort() {
			if (!Stopped) {
				{
					std::unique_lock<std::mutex> lock(ThreadMutex);
					IsRunning.store(false); // use this flag in condition.wait
				}
				condition.notify_all(); // wake up all threads.

				Join();

				Stopped = true; // use this flag in destructor, if not set, call shutdown() 
			}
		}
		void Submit(std::function<void()> func) {
			funcQueue.push_back(func);
			condition.notify_one();
		}
		template<typename T>
		void Submit(T* instance, void(T::* f)()) {
			funcQueue.push_back([instance]() {(instance->*f)(); });
			condition.notify_one();
		}
	};
}