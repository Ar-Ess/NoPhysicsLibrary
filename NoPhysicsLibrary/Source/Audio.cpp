#include "Audio.h"
#include <assert.h>
#include "Define.h"

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

void Audio::Playback(SoundData* data)
{
    ma_node* lastNode = nullptr;

    // Create new sound
    Sound* sound = new Sound(new ma_sound(), nullptr);
    ma_sound_init_copy(&engine, sounds[data->index], 0, NULL, sound->source);

    // Set sound panninig
    ma_sound_set_pan(sound->source, data->pan);

    // Set sound volume
    ma_sound_set_volume(sound->source, data->volume);

    // Set sound delay (the first one, attached to engine input bus)
    if (data->delayTime > 0)
    {
        sound->delay = new ma_delay_node();

        ma_delay_node_config delayNodeConfig;
        ma_uint32 channels;
        ma_uint32 sampleRate;

        channels = ma_engine_get_channels(&engine);
        sampleRate = ma_engine_get_sample_rate(&engine);

        //                                                                                     Delay Time         Falloff
        delayNodeConfig = ma_delay_node_config_init(channels, sampleRate, (ma_uint32)(sampleRate * data->delayTime), 0.0f);

        ma_delay_node_init(ma_engine_get_node_graph(&engine), &delayNodeConfig, NULL, sound->delay);

        ma_delay_node_set_dry(sound->delay, 0);

        /* Connect the output of the delay node to the input of the endpoint. */
        ma_node_attach_output_bus(sound->delay, 0, ma_engine_get_endpoint(&engine), 0);

        lastNode = sound->delay;
    }

    // Attach first node to sound output
    if (lastNode) // If any effect applied
    {
        /* Connect the output of the sound to the input of the effect. */
        ma_node_attach_output_bus(sound->source, 0, sound->delay, 0);
    }

    // Play sound
    ma_sound_start(sound->source);

    // Save sound reference
    playback.emplace_back(sound);
}

void Audio::Update()
{
    if (playback.empty()) return;

    size_t size = playback.size();
    for (unsigned int i = 0; i < size; ++i)
    {
        Sound* s = playback[i];

        if (ma_sound_at_end(s->source))
        {
            playback.erase(playback.begin() + i);
            RELEASE(s);
            --size;
            --i;
        }
    }
}

void Audio::LoadSound(const char* path)
{
    ma_sound* source = new ma_sound();
    ma_sound_init_from_file(&engine, path, 0, NULL, NULL, source);

    sounds.emplace_back(source);

    source = nullptr;
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
        for (ma_sound* s : sounds)
        {
            ma_sound_stop(s);
            ma_sound_uninit(s);
            RELEASE(s);
        }
    }
    sounds.clear();
    sounds.shrink_to_fit();

    ma_engine_uninit(&engine);
}
