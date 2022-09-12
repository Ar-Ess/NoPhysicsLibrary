#pragma once

#include "External\dr_libs\dr_mp3.h"
#include "Sound.h"
#include "SoundData.h"
#include "Define.h"
#include <vector>

class Audio
{
public:

	Audio();

	~Audio();

	void Update();

	void PushSound(int index);

	void LoadSound(const char* path);

	void HowToModulateVolumeAndPan(float distance);

	void CleanUp();

private:

	ma_engine engine;
	std::vector<Sound*> sounds;
	std::vector<SoundData*> soundList;

};
