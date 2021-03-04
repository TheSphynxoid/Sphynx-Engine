#include "pch.h"
#include "SpTime.h"

Sphynx::Timer::Timer()
{
	timer = clock();
	StartTimer = timer;
}

Sphynx::Timer::~Timer()
{
}

void Sphynx::Timer::Tick()
{
	timer = clock();
}

long Sphynx::Timer::TickAndReturnOld()
{
	long curr = GetTicks();
	Tick();
	return curr;
}

long Sphynx::Timer::GetTicks()
{
	return timer;
}

//Sphynx::Time::Time()
//{
//}

void Sphynx::Time::Start(Application* app)
{
	timer = Timer();
}

void Sphynx::Time::Update()
{
	double old = timer.GetTicks();
	timer.Tick();
	DeltaTime = timer.GetTicks() - old;
}

double Sphynx::Time::GetDeltaTime()
{
	return DeltaTime;
}

long Sphynx::Time::GetActivityTime()
{
	return timer.GetTicks();
}
