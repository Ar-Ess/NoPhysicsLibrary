#pragma once

#include "External/miniaudio/miniaudiodev.h"

struct SoundLoad
{
	SoundLoad(ma_sound* sound) : sound(sound) {}

	ma_sound* sound = nullptr;
};
