#pragma once
#include <time.h>
#include "Core/Module.h"

namespace Sphynx {
	class Timer {
	private:
		clock_t timer;
		clock_t StartTimer;
	public:
		Timer();
		~Timer();
		void Tick();
		long TickAndReturnOld();
		long GetTicks();
	};
	class Time : public Core::Module
	{
	private:
		double DeltaTime;
		Timer timer;
	public:
		//Time();
		void Start(Application* app)override;
		void Update()override;
		double GetDeltaTime();
		long GetActivityTime();
		Timer& GetTimer() { return timer; };
	};
}
