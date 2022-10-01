#pragma once

#include "External/miniaudio/miniaudio.h"

struct Timer
{
	Timer();

	void Start();

	int ReadSecs() const;

private:

	int GetSecs() const;

private:

	double startTime = 0.0f;
};
