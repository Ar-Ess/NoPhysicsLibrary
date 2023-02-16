#pragma once
#include "External/miniaudio/miniaudio.h"
//#include "External/miniaudio/extras/nodes/ma_reverb_node/ma_reverb_node.h"
#include "Timer.h"

class Sound
{
public:

	~Sound();

	void Play();

	void StartTimer();

	void SetPan(float pan);

	void SetVolume(float volume);

	ma_delay_node* ConnectDelay(ma_engine* engine, float delayTime);

	bool IsDelayOver()
	{
		if (!timer)
		{
			timer = new Timer();
			timer->Start();
		}
		return (timer->ReadSecs() >= timeUntilPlay && !played);
	}

	bool IsPlayed() const
	{
		return played;
	}

private:

	Sound(ma_sound* source, float timeToDelete);

public:

	ma_sound* source = nullptr;
	ma_delay_node* delay = nullptr;

private:

	friend class Audio;

	float timeUntilPlay = 0.0f;
	Timer* timer = nullptr;
	bool played = false;

};