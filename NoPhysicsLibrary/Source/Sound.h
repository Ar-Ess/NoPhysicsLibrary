#pragma once
#include "External/miniaudio/miniaudio.h"

class Sound
{
public:

	Sound(ma_sound* source);

	~Sound();

public:
	
	ma_sound* source = nullptr;

};