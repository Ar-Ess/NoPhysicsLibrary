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
    //-TODONE: put this while processiong acoustic data. Delete acoustic data with this kind of indexes
    // This index does not exist
    ma_node* lastNode = nullptr;
    
    // Delay Logic Approach (not totally working)
    //Sound* sound = new Sound(new ma_sound(), (sounds[data->index]->length * 1.1f) + data->delayTime);

    // Timer Logic Approach (if final, do not need to calculate sound length)
    Sound* sound = new Sound(new ma_sound(), data->delayTime);

    // Create new sound
    ma_sound_init_copy(&engine, sounds[data->index]->sound, 0, NULL, sound->source);

    // Set sound panning
    sound->SetPan(data->pan);

    // Set sound volume
    sound->SetVolume(data->volume);

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

        if (s->IsOver()) s->Play();

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
