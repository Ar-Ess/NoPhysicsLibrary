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
{
}

void Audio::Playback(SoundData* data)
{
    ma_sound* sound = new ma_sound();
    ma_sound_init_copy(&engine, sounds[data->index]->source, 0, NULL, sound);

    ma_sound_set_pan(sound, data->pan);
    ma_sound_set_volume(sound, data->volume);
    ma_sound_start(sound);

    playback.emplace_back(sound);
}

void Audio::Update()
{
    if (playback.empty()) return;

    size_t size = playback.size();
    for (unsigned int i = 0; i < size; ++i)
    {
        ma_sound* s = playback[i];

        if (ma_sound_at_end(s))
        {
            playback.erase(playback.begin() + i);
            ma_sound_stop(s);
            ma_sound_uninit(s);
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

    sounds.emplace_back(new Sound(source));

    source = nullptr;
}

void Audio::CleanUp()
{
    if (!playback.empty())
    {
        for (ma_sound* s : playback) 
        {
            ma_sound_stop(s);
            ma_sound_uninit(s);
            RELEASE(s);
        }
    }

    if (!sounds.empty())
    {
        for (Sound* s : sounds) RELEASE(s);
    }

    ma_engine_uninit(&engine);

    sounds.clear();
    sounds.shrink_to_fit();
}
