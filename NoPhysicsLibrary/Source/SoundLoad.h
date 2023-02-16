#pragma once

struct SoundLoad
{
private: SoundLoad(ma_sound* sound) : sound(sound) {}
public: ma_sound* sound = nullptr;
private: friend class Audio;
};
