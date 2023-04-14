#include "Audio.h"
#include "SoundData.h"

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

    audio->playClocked(data->delayTime, *sounds[data->index]->sound, data->volume, data->pan);

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
