#pragma once
#include "External/miniaudio/miniaudio.h"
#include "Timer.h"

class Sound
{
public:

	Sound(ma_sound* source);

	~Sound();

	void Play();

	void SetPan(float pan);

	void SetVolume(float volume);

	ma_delay_node* ConnectDelay(ma_engine* engine, float delayTime);

public:
	
	ma_sound* source = nullptr;
	ma_delay_node* delay = nullptr;

private:

	Timer* timer = nullptr;

};