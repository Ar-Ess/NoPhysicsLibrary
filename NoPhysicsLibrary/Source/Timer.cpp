#include "Timer.h"
#include <ctime>

Timer::Timer()
{
}

void Timer::Start()
{
	startTime = GetSecs();
}

int Timer::ReadSecs() const
{
	if (startTime == -1) return 0;

	return GetSecs() - startTime;
}

int Timer::GetSecs() const
{
	return int(clock() / CLOCKS_PER_SEC);
}
