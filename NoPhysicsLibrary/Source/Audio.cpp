#include "Audio.h"
#include "Log.h"
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
}

Audio::~Audio()
{}

void Audio::LoadAudio(const char* filePath)
{
    ma_result result;
    ma_device_config config;

    result = ma_decoder_init_file(filePath, NULL, &decoder);
    
    assert(result == MA_SUCCESS, "Could not load file: %s\n", filePath);

    config = ma_device_config_init(ma_device_type_playback);
    config.playback.format = decoder.outputFormat;
    config.playback.channels = decoder.outputChannels;
    config.sampleRate = decoder.outputSampleRate;
    config.dataCallback = DataCallback;
    config.pUserData = &decoder;


    if (ma_device_init(NULL, &config, &device) != MA_SUCCESS)
    {
        ma_decoder_uninit(&decoder);
        assert("Failed to open playback device.\n");
    }
}

void Audio::LoadAudioSound(const char* filePath)
{
    ma_engine_init(NULL, &engine);

    ma_sound_init_from_file(&engine, filePath, 0, NULL, NULL, &sound);
}

void Audio::PlayAudio()
{
    ma_device_start(&device);
}

void Audio::PlayAudioSound(float distance, bool shiftVolume, bool shiftBoth)
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
            ma_sound_set_volume(&sound, 1);
            ma_sound_set_pan(&sound, pan);
        }
        else
        {
            ma_sound_set_volume(&sound, volume);
            ma_sound_set_pan(&sound, 0);
        }
    }
    else
    {
        ma_sound_set_pan(&sound, pan);
        ma_sound_set_volume(&sound, volume);
    }

    ma_sound_start(&sound);
}

void Audio::UnloadAudio()
{
    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);
}

void Audio::UnloadAudioSound()
{
    ma_sound_uninit(&sound);
    ma_engine_uninit(&engine);
}
