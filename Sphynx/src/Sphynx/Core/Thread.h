#pragma once
#include <thread>
#include <atomic>
#include <mutex>
#include <functional>

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
		std::thread::id GetID() const noexcept {
			return thread.get_id();
		}
		template<typename fty>
		void SetJob(std::function<fty> func) {
			thread = std::thread(func);
		}
	};
	template<typename Mutex = std::mutex>
	class LoopThread : public Thread {
	private:
		std::atomic<bool> IsRunning = false;
		std::deque<std::function<void()>> funcQueue;
		std::condition_variable condition;
		bool Stopped = false;
		Mutex ThreadMutex;
		void Loop() {
			std::unique_lock<Mutex> ul(ThreadMutex);
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
					std::unique_lock<Mutex> lock(ThreadMutex);
					IsRunning.store(false); // use this flag in condition.wait
				}
				condition.notify_all(); // wake up all threads.

				Join();

				Stopped = true; // use this flag in destructor, if not set, call shutdown() 
			}
		}
		void Submit(std::function<void()> func) {
			//By my logic we don't need a mutex because this doesn't cause any race condition on the call thread
			//There is nothing to wait for before adding a job
			funcQueue.push_back(func);
			condition.notify_one();
		}
		template<typename T>
		void Submit(T* instance, void(T::* f)()) {
			//By my logic we don't need a mutex because this doesn't cause any race condition on the call thread
			//There is nothing to wait for before adding a job
			funcQueue.push_back([instance]() {(instance->*f)(); });
			condition.notify_one();
		}
	};
}