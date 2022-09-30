#pragma once
#include "External/miniaudio/miniaudio.h"
#include "Timer.h"

class Sound
{
public:

	Sound(ma_sound* source, Timer* timer);

	~Sound();

public:
	
	ma_sound* source = nullptr;
	ma_delay_node* delay = nullptr;
	Timer* timer = nullptr;

};