#pragma once

#include "External/SoLoud/include/soloud_wav.h"
#include "External/SoLoud/include/soloud_biquadresonantfilter.h"
#include "PitchShiftFilter.h"

class SoLoud::Wav;
class SoLoud::BiquadResonantFilter;
class PitchShiftFilter;

class Sound
{

	friend class Audio;

public:

	~Sound();

private:

	Sound(SoLoud::BiquadResonantFilter* lowpass, PitchShiftFilter* pitch);
	
	bool Load(const char* path);

	void Lowpass(float frequency, float resonance);

	void Pitch(float pitchOffset);

private:

	SoLoud::Wav* sound = nullptr;
	SoLoud::BiquadResonantFilter* lowpass = nullptr;
	PitchShiftFilter* pitch = nullptr;

};