#include "Sound.h"

Sound::Sound(ma_sound* source, float length)
{
	this->source = source;
	this->length = length;
	timer = new Timer();
}

void Sound::Play()
{
	ma_sound_start(source);
	timer->Start();
}

void Sound::SetPan(float pan)
{
	ma_sound_set_pan(source, pan);
}

void Sound::SetVolume(float volume)
{
	ma_sound_set_volume(source, volume);
}

ma_delay_node* Sound::ConnectDelay(ma_engine* engine, float delayTime, float falloff)
{
	delay = new ma_delay_node();

	ma_delay_node_config delayNodeConfig;
	ma_uint32 channels;
	ma_uint32 sampleRate;

	channels = ma_engine_get_channels(engine);
	sampleRate = ma_engine_get_sample_rate(engine);

	//                                                                                     Delay Time         Falloff
	delayNodeConfig = ma_delay_node_config_init(channels, sampleRate, (ma_uint32)(sampleRate * delayTime), falloff);

	ma_delay_node_init(ma_engine_get_node_graph(engine), &delayNodeConfig, NULL, delay);

	ma_delay_node_set_dry(delay, 0);

	/* Connect the output of the delay node to the input of the endpoint. */
	ma_node_attach_output_bus(delay, 0, ma_engine_get_endpoint(engine), 0);

	return delay;
}

Sound::~Sound()
{
	ma_sound_stop(source);
	ma_sound_uninit(source);
	delete source;
	source = nullptr;

	ma_delay_node_uninit(delay, NULL);
	delete delay;
	delay = nullptr;
}

