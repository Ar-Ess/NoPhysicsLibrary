#include "Sound.h"
#include "soloud_biquadresonantfilter.h"

Sound::Sound(SoLoud::BiquadResonantFilter* lowpass, PitchShiftFilter* pitch)
{
	sound = new SoLoud::Wav();

	this->pitch = pitch;
	sound->setFilter(0, pitch);

	this->lowpass = lowpass;
	sound->setFilter(1, lowpass);

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

void Sound::Pitch(float pitchOffset)
{
	if (pitchOffset > 10) pitchOffset = 10;
	pitch->SetPitchShift(pitchOffset);
}
