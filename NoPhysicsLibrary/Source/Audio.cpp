#include "Audio.h"
#include <assert.h>
#include "Define.h"
#include "Timer.h"

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
{
}

void Audio::Playback(SoundData* data, float* dt)
{
    ma_node* lastNode = nullptr;

    // Create new sound
    Sound* sound = new Sound(new ma_sound(), sounds[data->index]->length + (data->delayTime * 1.1f));
    ma_sound_init_copy(&engine, sounds[data->index]->sound, 0, NULL, sound->source);

    // Set sound panning
    sound->SetPan(data->pan);

    // Set sound volume
    sound->SetVolume(data->volume);

    // Set sound delay (the first one, attached to engine input bus)
    if (data->delayTime > 0) lastNode = sound->ConnectDelay(&engine, data->delayTime);

    // Attach first node to sound output
    if (lastNode) // If any effect applied
    {
        /* Connect the output of the sound to the input of the effect. */
        ma_node_attach_output_bus(sound->source, 0, lastNode, 0);
    }

    // Play sound
    sound->Play();

    // Save sound reference
    playback.emplace_back(sound);
}

void Audio::Update()
{
    if (playback.empty()) return;
    static int test = 0;
    size_t size = playback.size();
    for (unsigned int i = 0; i < size; ++i)
    {
        Sound* s = playback[i];
        if (s->IsOver())
        {
            playback.erase(playback.begin() + i);
            RELEASE(s);
            --size;
            --i;
        }
    }
    test++;
}

void Audio::LoadSound(const char* path)
{
    //-TODONE: Calculate sound length
    ma_sound* sound = new ma_sound();
    ma_sound_init_from_file(&engine, path, 0, NULL, NULL, sound);
    float length = 0.0f;
    ma_sound_get_length_in_seconds(sound, &length);

    sounds.emplace_back(new SoundLoad(sound, length));

    sound = nullptr;
}

void Audio::CleanUp()
{
    if (!playback.empty())
    {
        for (Sound* s : playback) RELEASE(s);
    }
    playback.clear();
    playback.shrink_to_fit();

    if (!sounds.empty())
    {
        for (SoundLoad* sL : sounds)
        {
            ma_sound_stop(sL->sound);
            ma_sound_uninit(sL->sound);
            RELEASE(sL);
        }
    }
    sounds.clear();
    sounds.shrink_to_fit();

    ma_engine_uninit(&engine);
}
