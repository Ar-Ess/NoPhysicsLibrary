#pragma once

#include "External/miniaudio/miniaudio.h"

class Sound
{
public:

	Sound(ma_sound* source);

public:
	
	ma_sound* source = nullptr;

};