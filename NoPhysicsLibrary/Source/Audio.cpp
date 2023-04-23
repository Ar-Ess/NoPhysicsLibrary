#include "Audio.h"
#include "SoundData.h"
#include "NoReverbFilter.h"

SoLoud::NoReverbFilter* filter = nullptr;

Audio::Audio()
{
    audio = new SoLoud::Soloud();
    audio->init();

    filter = new SoLoud::NoReverbFilter(16);
    filter->setParams(0.5, 0.5, 0.5, 1);
}

Audio::~Audio()
{
}

void Audio::Playback(SoundData* data, float* dt)
{
    if (SoundSize() == 0 || data->index < 0 || data->index >= SoundSize()) return;

    SoLoud::Wav* sound = sounds[data->index]->sound;

    sound->setFilter(0, filter);

    audio->playClocked(data->delayTime, *sound, data->volume, data->pan);

}

void Audio::LoadSound(const char* path)
{
    Sound* sound = new Sound();

    if (sound->Load(path)) sounds.Add(sound);
    else delete sound;
}

void Audio::CleanUp()
{
    sounds.Clear();

    audio->deinit();
    delete audio;
}
