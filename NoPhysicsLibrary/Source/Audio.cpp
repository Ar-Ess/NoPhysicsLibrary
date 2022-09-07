#include "Audio.h"
#include <assert.h>

// Internal usage of miniaudio
void DataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == nullptr) return;

    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

    (void)pInput;
}

Audio::Audio()
{
    ma_engine_init(NULL, &engine);
}

Audio::~Audio()
{}

void Audio::Update()
{
    if (soundList.empty()) return;

    for (SoundData* sD : soundList)
    {
        ma_sound_start(sounds[sD->sound]->source);
    }

    soundList.clear();
    soundList.shrink_to_fit();
}

void Audio::PushSound(int index)
{
    // There is not a loaded testSound in "index" position
    assert(index < sounds.size() && index >= 0);

    soundList.push_back(new SoundData(index));
}

void Audio::LoadSound(const char* path)
{
    ma_sound* sound = new ma_sound();

    ma_sound_init_from_file(&engine, path, 0, NULL, NULL, sound);

    sounds.push_back(new Sound(sound));
}

void Audio::TestLoadAudio(const char* filePath)
{
    ma_sound_init_from_file(&engine, filePath, 0, NULL, NULL, &testSound);
}

void Audio::TestPlayAudio(float distance, bool shiftVolume, bool shiftBoth)
{
    if (distance > 1000) distance = 1000;
    if (distance < -1000) distance = -1000;

    float pan = (distance * 1) / -1000;
    float volume = (distance * 1) / 1000;
    if (volume < 0) volume *= -1;
    volume = 1 - volume;

    if (!shiftBoth)
    {
        if (!shiftVolume)
        {
            ma_sound_set_volume(&testSound, 1);
            ma_sound_set_pan(&testSound, pan);
        }
        else
        {
            ma_sound_set_volume(&testSound, volume);
            ma_sound_set_pan(&testSound, 0);
        }
    }
    else
    {
        ma_sound_set_pan(&testSound, pan);
        ma_sound_set_volume(&testSound, volume);
    }

    ma_sound_start(&testSound);
}

void Audio::TestUnloadAudio()
{
    ma_sound_uninit(&testSound);
}
