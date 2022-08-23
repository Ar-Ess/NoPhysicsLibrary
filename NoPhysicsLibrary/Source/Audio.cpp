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
    ma_decoder decoder;
    ma_device_config deviceConfig;
    ma_device device;

    result = ma_decoder_init_file(filePath, NULL, &decoder);
    
    assert(result == MA_SUCCESS, "Could not load file: %s\n", filePath);

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.sampleRate = decoder.outputSampleRate;
    deviceConfig.dataCallback = DataCallback;
    deviceConfig.pUserData = &decoder;


    if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) 
    {
        ma_decoder_uninit(&decoder);
        assert("Failed to open playback device.\n");
    }

    // play
    if (ma_device_start(&device) != MA_SUCCESS) 
    {
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        assert("Failed to start playback device.\n");
    }

    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);
}