#include "Timer.h"

Timer::Timer(ma_engine* engine)
{
	this->engine = engine;
}

void Timer::Start() const
{
	startTime = ma_engine_get_time(engine,);
}

long int Timer::ReadSecs() const
{
}
