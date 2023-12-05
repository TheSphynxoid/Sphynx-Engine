#include "pch.h"
#include "SpTime.h"
#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

float Sphynx::Time::GetDeltaTime() noexcept
{
	return DeltaTime.count();
}

unsigned __int64 Sphynx::Time::GetDeltaTicks()
{
	return diff_tick;
}

unsigned __int64 Sphynx::Time::GetTicks()
{
	return __rdtsc();
}

void Sphynx::Time::Start()
{
	last = timer::now();
	last_tick = __rdtsc();
}

void Sphynx::Time::Update()
{
	auto cycles = __rdtsc();
	time_point curr = timer::now();
	DeltaTime = curr - last;
	diff_tick = cycles - last_tick;
	last_tick = cycles;
	last = curr;
}
