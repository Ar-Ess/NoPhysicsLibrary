#pragma once

#include "External/miniaudio/miniaudiodev.h"

struct SoundLoad
{
	SoundLoad(ma_sound* sound, float length) : sound(sound), length(length) {}

	ma_sound* sound = nullptr;
	float length = 0.0f;
};
