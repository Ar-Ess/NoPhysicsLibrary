// ----------------------------------------------------
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "LightTimer.h"
#include "SDL\include\SDL_timer.h"

// L07: DONE 1: Fill Start(), Read(), ReadSec() methods
// they are simple, one line each!
	
LightTimer::LightTimer()
{
	Start();
}

void LightTimer::Start()
{
	startTime = SDL_GetTicks();
}

uint32 LightTimer::Read() const
{
	return SDL_GetTicks() - startTime;
}

double LightTimer::ReadSec() const
{
	return (SDL_GetTicks() - startTime) / 1000.0;
}
