#pragma once
#include "External/miniaudio/miniaudio.h"
#include "Timer.h"

class Sound
{
public:

	Sound(ma_sound* source, float length);

	~Sound();

	void Play();

	void SetPan(float pan);

	void SetVolume(float volume);

	ma_delay_node* ConnectDelay(ma_engine* engine, float delayTime, float falloff);

public:
	
	ma_sound* source = nullptr;
	ma_delay_node* delay = nullptr;
	float timeToDelete = 0.0f;

private:

	Timer* timer = nullptr;
	float length = 0.0f;

};