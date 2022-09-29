#include "Sound.h"

Sound::Sound(ma_sound* source)
{
	this->source = source;
}

Sound::~Sound()
{
	ma_sound_stop(source);
	ma_sound_uninit(source);
	delete source;
	source = nullptr;

	ma_delay_node_uninit(delay, NULL);
	delete delay;
	delay = nullptr;
}

