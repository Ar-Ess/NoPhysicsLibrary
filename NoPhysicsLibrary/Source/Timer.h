#pragma once

#include "External/miniaudio/miniaudio.h"

struct Timer
{
	Timer(ma_engine* engine);

	void Start() const;

	ma_uint64 ReadSecs() const;

private:

	ma_uint64 startTime = 0;
	ma_engine* engine = nullptr;
};
