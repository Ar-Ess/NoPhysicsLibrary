#pragma once

#include "External/SoLoud/include/soloud_wav.h"

class Sound
{

	friend class Audio;

public:

	~Sound();

	bool Load(const char* path);

private:

	Sound();

private:

	SoLoud::Wav* sound = nullptr;

};