#pragma once
#include <chrono>
#include <cstdint>
#include "Core/Module.h"
#define timer std::chrono::steady_clock


namespace Sphynx {
	class Time final {
	private:
		typedef std::chrono::duration<float> time;
		typedef std::chrono::time_point<timer> time_point;
		inline static time_point last;
		inline static std::uint64_t last_tick;
		inline static std::uint64_t diff_tick;
		inline static time DeltaTime;
	public:
		static float GetDeltaTime() noexcept;
		static unsigned __int64 GetDeltaTicks();
		static unsigned __int64 GetTicks();
		// Inherited via Module
		static void Start();
		static void Update();
	};
}
