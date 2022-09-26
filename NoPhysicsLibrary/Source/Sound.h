#pragma once
#include "External/miniaudio/miniaudio.h"

class Sound
{
public:

	Sound(ma_sound* source, ma_delay_node* delay);

	~Sound();

public:
	
	ma_sound* source = nullptr;
	ma_delay_node* delay = nullptr;

};