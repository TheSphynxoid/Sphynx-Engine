#include "pch.h"
#include "SpTime.h"

double Sphynx::Time::GetDeltaTime()
{
	return DeltaTime.count();
}

void Sphynx::Time::Start()
{
	last = timer::now();
}

void Sphynx::Time::Update()
{
	time_point curr = timer::now();
	DeltaTime = curr - last;
	last = curr;
}
