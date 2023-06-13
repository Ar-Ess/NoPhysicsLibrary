#include "Sound.h"
#include "soloud_biquadresonantfilter.h"

Sound::Sound(SoLoud::BiquadResonantFilter* lowpass)
{
	sound = new SoLoud::Wav();

	this->lowpass = lowpass;
	sound->setFilter(0, lowpass);
}

Sound::~Sound()
{
	delete sound;
}

bool Sound::Load(const char* path)
{
	return sound->load(path) == 0;
}

void Sound::Lowpass(float frequency, float resonance)
{
	lowpass->setParams(SoLoud::BiquadResonantFilter::FILTERTYPE::LOWPASS, frequency, resonance);
}
