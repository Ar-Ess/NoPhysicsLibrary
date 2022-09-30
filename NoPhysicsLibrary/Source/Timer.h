#pragma once

#include "External/miniaudio/miniaudio.h"

struct Timer
{
	Timer(ma_engine* engine, float* dt);

	void Start();

	ma_uint64 ReadSecs() const;

private:

	double startTime = 0.0f;
	ma_engine* engine = nullptr;
	float* dt = nullptr;
};
