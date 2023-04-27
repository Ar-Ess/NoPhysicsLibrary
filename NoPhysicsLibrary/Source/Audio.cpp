#include "Audio.h"
#include "SoundData.h"

#define SEC_TO_SAMPLES(seconds) seconds * 44100

Audio::Audio()
{
    audio = new SoLoud::Soloud();
    audio->init();
}

Audio::~Audio()
{
}

void Audio::Playback(SoundData* data, float* dt)
{
    if (SoundSize() == 0 || data->index < 0 || data->index >= SoundSize()) return;

    bool delay = data->delayTime > 0.1;
    SoLoud::handle h = audio->play(*sounds[data->index]->sound, data->volume, data->pan, delay);
    if (!delay) return;

    audio->setDelaySamples(h, SEC_TO_SAMPLES(data->delayTime));
    audio->setPause(h, 0);
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
