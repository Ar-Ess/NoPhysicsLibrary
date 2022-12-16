#ifndef __TIMER_H__
#define __TIMER_H__

#include "Defs.h"

class LightTimer
{
public:

	// Constructor
	LightTimer();

	void Start();
	uint32 Read() const;
	double ReadSec() const;

private:
	uint32 startTime;
};

#endif //__TIMER_H__