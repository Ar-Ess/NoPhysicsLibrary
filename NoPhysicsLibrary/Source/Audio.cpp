#include "Audio.h"
#include "SoundData.h"

#define SEC_TO_SAMPLES(seconds) seconds * 44100

Audio::Audio()
{
    audio = new SoLoud::Soloud();
    audio->init();

    lowpass = new SoLoud::BiquadResonantFilter();
    lowpass->setParams(SoLoud::BiquadResonantFilter::LOWPASS, 22000, 2);
}

Audio::~Audio()
{
}

void Audio::Playback(SoundData* data, float* dt)
{
    if (SoundSize() == 0 || data->index < 0 || data->index >= SoundSize()) return;

    bool delay = data->delayTime > 0.1;
    Sound* sound = sounds[data->index];

    sound->Lowpass(data->frequency, data->resonance);

    SoLoud::handle h = audio->play(*sound->sound, data->volume, data->pan, delay);
    
    if (!delay) return;

    audio->setDelaySamples(h, SEC_TO_SAMPLES(data->delayTime));
    audio->setPause(h, false);
}

void Audio::LoadSound(const char* path)
{
    Sound* sound = new Sound(lowpass);

    if (sound->Load(path)) sounds.Add(sound);
    else delete sound;

}

void Audio::CleanUp()
{
    sounds.Clear();

    audio->deinit();
    delete audio;
}
