#include "Audio.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

Audio::Audio(AssetsManager* assets) : Module()
{
	this->assets = assets;
}

Audio::~Audio()
{}

bool Audio::Start()
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// Load support for the OGG formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	// Initialize SDL_mixer
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	Mix_AllocateChannels(360);
	SetChannelAngles();

	Mix_VolumeMusic(128);

	return ret;
}

bool Audio::CleanUp()
{
	if(!active) return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	suint size = music.size();
	for (suint i = 0; i < size; ++i)
		Mix_FreeMusic(music[i]);

	size = sfx.size();
	for(suint i = 0; i < size; ++i)
		Mix_FreeChunk(sfx[i]);

	sfx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

bool Audio::LoadTrack(const char* path)
{
	if (!active) return false;

	SDL_RWops* rW = assets->LoadAsset(path);
	Mix_Music* track = Mix_LoadMUS_RW(rW, 0);

	if (!track)
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	else
		music.push_back(track);

	return true;
}

bool Audio::LoadSfx(const char* path)
{
	if (!active) return false;

	SDL_RWops* rW = assets->LoadAsset(path);
	Mix_Chunk* chunk = Mix_LoadWAV_RW(rW, 0);

	if (!chunk) 
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	else
		sfx.push_back(chunk);

	return true;
}

bool Audio::PlayTrack(Track track, float fadeTime)
{
	bool ret = true;
	if (!active || track == Track::NO_TRACK) return false;
	Mix_Music* m = music[track];

	if (Mix_PlayingMusic())
	{
		if (fadeTime > 0.0f)
			Mix_FadeOutMusic(int(fadeTime * 1000.0f));
		else
			Mix_HaltMusic();
	}

	if (fadeTime > 0.0f) 
		if (Mix_FadeInMusic(m, -1, (int)(fadeTime * 1000.0f)) < 0) ret = false;
	else
		if (Mix_PlayMusic(m, -1) < 0) ret = false;

	return ret;
}

uint Audio::GetAngle(Point p1, Point p2)
{
	Point vec(p2.x - p1.x, p2.y - p1.y);

	float dot = (0 * vec.x) + (1 * vec.y);
	float det = (0 * vec.y) - (1 * vec.x);

	double angle = atan2(det, dot) * RADS_TO_DEG;
	angle += 180.0f;

	if (angle < 0)
		angle += 180.0f;

	uint ret = static_cast<uint>(angle);

	return ret;
}

uint Audio::GetVolumeFromDistance(Point p1, Point p2)
{
	Point vec(p2.x - p1.x, p2.y - p1.y);
	float screenDist = sqrt(pow(vec.x, 2) + pow(vec.y, 2));

	if (screenDist >= MUTE_DISTANCE)
		return uint(MUTE_DISTANCE_VOL);

	float scaledDist = screenDist * (MUTE_DISTANCE_VOL / MAX_DISTANCE);
	uint volume = static_cast<uint>(scaledDist);

	if (volume > MAX_DISTANCE_VOL)
		volume = MAX_DISTANCE_VOL;

	return volume;
}

void Audio::SetChannelAngles()
{
	for (suint i = 0; i <= 360; i++) 
	{
		Mix_SetPosition(i, i, 1);
	}
}

int Audio::GetMusicVolume()
{
	return Mix_VolumeMusic(-1);
}

int Audio::GetFxVolume(suint index)
{
	return Mix_VolumeChunk(sfx[index], -1);
}

void Audio::ChangeVolumeMusic(suint volume)
{
	volume = ValueToVolume(volume);

	if (volume < 0) volume = 0;
	else if (volume > 128) volume = 128;

	Mix_VolumeMusic(volume);
}

void Audio::ChangeVolumeFx(suint index, suint volume)
{
	if (index >= sfx.size()) return;

	volume = ValueToVolume(volume);

	if (volume < 0) volume = 0;
	else if (volume > 128) volume = 128;
	
	Mix_VolumeChunk(sfx[index], volume);
}

int Audio::ValueToVolume(int value, int maxPercent)
{
	return ((128 * value) / maxPercent);
}

int Audio::VolumeToValue(int volume, int maxPercent)
{
	return ((maxPercent * volume) / 128);
}

void Audio::TogglePauseMusic()
{
	if (Mix_PausedMusic)
		Mix_PauseMusic();
	else
		Mix_ResumeMusic();
}

void Audio::StopMusic()
{
	Mix_HaltMusic();
}

bool Audio::PlaySfx(Sfx fx)
{
	bool ret = false;

	if(!active || fx == NO_SFX) return false;
	
	
	Mix_PlayChannel(-1, sfx[fx], 0);

	return ret;
}

bool Audio::PlayFxOnChannel(uint index, uint channel, uint distance, int repeat)
{
	bool ret = true;

	if (!active) return false;

	if (sfx[index])
	{
		while (Mix_Playing(channel))
		{
			channel++;
			if (channel >= MAX_CHANNELS)
				channel = 0;
		}

		Mix_SetPosition(channel, channel, distance);
		Mix_PlayChannel(channel, sfx[index], repeat);
	}
	else
		ret = false;
	return ret;
}
