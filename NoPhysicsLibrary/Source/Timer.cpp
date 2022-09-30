#include "Timer.h"

Timer::Timer(ma_engine* engine, float* dt)
{
	this->engine = engine;
	this->dt = dt;
}

void Timer::Start()
{
	startTime = ma_engine_get_time(engine) * (double)(*dt);
}

ma_uint64 Timer::ReadSecs() const
{
	return (ma_engine_get_time(engine) * (double)(*dt)) - startTime;
}
// Question:
// el "dt" d'un videojoc es seconds/frame right?
