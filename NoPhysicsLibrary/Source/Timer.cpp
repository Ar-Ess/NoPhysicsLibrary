#include "Timer.h"
#include <ctime>

Timer::Timer()
{
}

void Timer::Start()
{
	startTime = int(clock());
}

int Timer::ReadSecs() const
{
	return int(clock()) - startTime;
}
