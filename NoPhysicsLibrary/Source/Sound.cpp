#include "Sound.h"

Sound::Sound()
{
	sound = new SoLoud::Wav();
}

Sound::~Sound()
{
	delete sound;
}

bool Sound::Load(const char* path)
{
	return sound->load(path) == 0;
}
