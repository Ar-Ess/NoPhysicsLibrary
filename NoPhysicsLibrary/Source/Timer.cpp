#include "Timer.h"
#include <ctime>

Timer::Timer()
{
}

void Timer::Start()
{
	startTime = int(clock()) / CLOCKS_PER_SEC;
}

int Timer::ReadSecs() const
{
	return (clock() / CLOCKS_PER_SEC) - startTime;
}
