#include "Audio.h"
#include "SoundData.h"

#define SEC_TO_SAMPLES(seconds) seconds * 44100
#define SOFTEN_PITCH(pitch) ((int)(pitch * 100))/100.0f
#include "Define.h"

Audio::Audio()
{
    audio = new SoLoud::Soloud();
    audio->init();

    lowpass = new SoLoud::BiquadResonantFilter();
    lowpass->setParams(SoLoud::BiquadResonantFilter::LOWPASS, 22000, 2);

    pitch = new PitchShiftFilter();
    pitch->SetPitchShift(1);
}

Audio::~Audio()
{
}

void Audio::Playback(SoundData* data)
{
    if (SoundSize() == 0 || data->index < 0 || data->index >= SoundSize()) return;

    bool delay = data->delayTime > 0.1;
    Sound* aSound = sounds[data->index];

    aSound->Lowpass(data->frequency, data->resonance);
    aSound->Pitch(SOFTEN_PITCH(data->pitch));

    SoLoud::handle v = audio->play(*aSound->sound, data->volume, data->pan, delay);
    
    if (!delay) return;

    unsigned int i = SEC_TO_SAMPLES(data->delayTime);
    if (delay) audio->setDelaySamples(v, i);

    bool delayPause = data->delayTime < -1;
    audio->setPause(v, delayPause);
}

bool Audio::LoadSound(const char* path)
{
    int i = sounds.Size();
    Sound* sound = new Sound(lowpass, pitch);

    if (sound->Load(path)) sounds.Add(sound);
    else delete sound;

    return sounds.Size() > i;
}

void Audio::CleanUp()
{
    audio->stopAll();

    sounds.Clear();

    audio->deinit();
    delete audio;
}
