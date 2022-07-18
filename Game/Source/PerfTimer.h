#pragma once

#include "Defs.h"

class PerfTimer
{
public:

	// Constructor
	PerfTimer();

	void Start();
	double ReadMs() const;
	uint64 ReadTicks() const;
	float ReadSec() const;

private:
	uint32	started_at;

	uint64 startTime;
	static uint64 frequency;
};