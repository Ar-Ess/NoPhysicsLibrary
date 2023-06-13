#pragma once

#include "External/SoLoud/include/soloud_wav.h"
#include "External/SoLoud/include/soloud_biquadresonantfilter.h"

class SoLoud::Wav;
class SoLoud::BiquadResonantFilter;

class Sound
{

	friend class Audio;

public:

	~Sound();

private:

	Sound(SoLoud::BiquadResonantFilter* lowpass);
	
	bool Load(const char* path);

	void Lowpass(float frequency, float resonance);

private:

	SoLoud::Wav* sound = nullptr;
	SoLoud::BiquadResonantFilter* lowpass = nullptr;

};