#include "Timer.h"
#include <ctime>

Timer::Timer()
{
}

void Timer::Start()
{
	startTime = GetSecs();
}

double Timer::ReadSecs() const
{
	if (startTime == -1) return 0;

	return GetSecs() - startTime;
}

double Timer::GetSecs() const
{
	return double(clock() / CLOCKS_PER_SEC);
}
