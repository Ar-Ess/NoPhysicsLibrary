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

void Audio::Update(SoundData* data)
{

    ma_sound* source = sounds[data->index]->source;

    ma_sound_set_pan(source, data->pan);
    ma_sound_set_volume(source, data->volume);
    ma_sound_start(source);

}

void Audio::PushSound(int index, Point position)
{
    // There is not a loaded testSound in "index" position
    assert(index < sounds.size() && index >= 0);

    //soundList.emplace_back(new SoundData(index, position));
}

void Audio::LoadSound(const char* path)
{
    ma_sound* sound = new ma_sound();

    ma_sound_init_from_file(&engine, path, 0, NULL, NULL, sound);

    sounds.emplace_back(new Sound(sound));
}

void Audio::CleanUp()
{
}
