#pragma once

#include "Defs.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	uint32 Read() const;
	float ReadSec() const;

private:
	uint32 startTime;
};