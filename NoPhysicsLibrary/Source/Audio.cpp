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

//void ReverbCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
//{
//    MA_ASSERT(pDevice->capture.format == pDevice->playback.format && pDevice->capture.format == ma_format_f32);
//    MA_ASSERT(pDevice->capture.channels == pDevice->playback.channels);
//
//    /*
//    The node graph system is a pulling style of API. At the lowest level of the chain will be a
//    node acting as a data source for the purpose of delivering the initial audio data. In our case,
//    the data source is our `pInput` buffer. We need to update the underlying data source so that it
//    read data from `pInput`.
//    */
//    ma_audio_buffer_ref_set_data(&g_dataSupply, pInput, frameCount);
//
//    /* With the source buffer configured we can now read directly from the node graph. */
//    ma_node_graph_read_pcm_frames(&g_nodeGraph, pOutput, frameCount, NULL);
//}

// Audio System
Audio::Audio()
{
    ma_engine_init(NULL, &engine);
}

Audio::~Audio()
{
}

void Audio::Playback(SoundData* data, float* dt)
{
    // This index does not exist
    ma_node* lastNode = nullptr;

    // Timer Logic Approach
    Sound* sound = new Sound(new ma_sound(), data->delayTime);

    // Create new sound
    ma_sound_init_copy(&engine, sounds[data->index]->sound, 0, NULL, sound->source);

    // Set sound panning
    sound->SetPan(data->pan);

    // Set sound volume
    sound->SetVolume(data->volume);

    //ma_reverb_node_config config = ma_reverb_node_config_init(ma_engine_get_channels(&engine), ma_engine_get_sample_rate(&engine));
    //// config congig_node with information about reverb
    //ma_reverb_node reverb;
    //ma_reverb_node_init(ma_engine_get_node_graph(&engine), &config, NULL, &reverb);

    // Delay Logic Approach (not totally working)
    /*
    // Set sound delay (the first one, attached to engine input bus)
    if (data->delayTime > 0.1f) lastNode = sound->ConnectDelay(&engine, data->delayTime);

    // Attach first node to sound output
    if (lastNode) // If any effect applied
    {
        // Connect the output of the sound to the input of the effect.
        ma_node_attach_output_bus(sound->source, 0, lastNode, 0);
    }

    // Play sound, this function does not include the timer->Start(), and it should with this approach
    sound->Play();
    */

    // Timer Logic Approach
    if (data->delayTime > 0.1f) 
        sound->StartTimer(); // Start the countdown
    else
        sound->Play(); // Play sound directly

    playback.emplace_back(sound); // Save sound reference
}

void Audio::Update()
{
    if (playback.empty()) return;
    size_t size = playback.size();
    for (unsigned int i = 0; i < size; ++i)
    {
        Sound* s = playback[i];

        // If the delay of the sound has ended, play the sound
        if (s->IsDelayOver()) s->Play();

        // If the sound is being played but it is at the end, delete it
        if (s->IsPlayed() && ma_sound_at_end(s->source))
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
    ma_sound* sound = new ma_sound();
    ma_sound_init_from_file(&engine, path, 0, NULL, NULL, sound);

    sounds.emplace_back(new SoundLoad(sound));

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
