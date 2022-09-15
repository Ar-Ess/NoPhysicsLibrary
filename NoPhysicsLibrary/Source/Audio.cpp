#include "Audio.h"
#include <assert.h>

// Deletes a buffer
#define RELEASE( x ) \
	{						\
	if( x != NULL )		\
		{					  \
	  delete x;			\
	  x = NULL;			  \
		}					  \
	}

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

void Audio::LoadSound(const char* path)
{
    ma_sound* sound = new ma_sound();

    ma_sound_init_from_file(&engine, path, 0, NULL, NULL, sound);

    sounds.emplace_back(new Sound(sound));
}

void Audio::CleanUp()
{
    ma_engine_uninit(&engine);

    if (!sounds.empty())
    {
        for (Sound* s : sounds) RELEASE(s);
    }

    sounds.clear();
    sounds.shrink_to_fit();
}
