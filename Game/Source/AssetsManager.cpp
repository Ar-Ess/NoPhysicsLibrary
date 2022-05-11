#include "AssetsManager.h"

#include "physfsrwops.h"
#include "physfsrwops.c"

AssetsManager::AssetsManager() : Module()
{
}

AssetsManager::~AssetsManager()
{
}

bool AssetsManager::Start()
{
	LOG("Loading Assets Manager");
	bool ret = true;

	if (PHYSFS_init(NULL) == 0)
	{
		LOG("ERROR INITIALIZING PHYSFS LIBRARY: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		ret = false;
	}

	if (PHYSFS_mount("Assets.pak", NULL, 1) == 0)
	{
		LOG("ERROR ADDING ARCHIVE TO SEARCH PATH: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		ret = false;
	}

	return ret;
}

SDL_RWops* AssetsManager::LoadAsset(const char* path)
{
	if (PHYSFS_exists(path) == 0)
	{
		LOG("ERROR - FILE %s DOESNT EXIST IN THE SEARCH PATH: %s\n", path, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return nullptr;
	}

	SDL_RWops* ret = PHYSFSRWOPS_openRead(path);

	if (!ret)
	{
		LOG("ERROR OPENING FILE %s FOR READING: %s\n", path, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return nullptr;
	}

	return ret;
}

size_t AssetsManager::LoadXML(const char* path, char** buffer)
{
	if (PHYSFS_exists(path) == 0)
		LOG("ERROR - FILE %s DOESNT EXIST IN THE SEARCH PATH: %s\n", path, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));

	PHYSFS_file* file = PHYSFS_openRead(path);
	if (file == NULL)
		LOG("ERROR OPENING FILE %s FOR READING: %s\n", path, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));

	if (!PHYSFS_eof(file))
	{
		PHYSFS_sint64 size = PHYSFS_fileLength(file);

		*buffer = new char[size];

		PHYSFS_sint64 numBytesRead = PHYSFS_readBytes(file, *buffer, size);
		if (numBytesRead == -1)
			LOG("ERROR READING FROM FILEHANDLE: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));

		if (numBytesRead == size)
		{
			if (PHYSFS_close(file) == 0)
				LOG("ERROR CLOSING FILEHANDLE: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));

			return numBytesRead;
		}
		else
		{
			PHYSFS_close(file);
			RELEASE_ARRAY(buffer);
			return 0;
		}
	}
}

bool AssetsManager::Cleanup()
{
	LOG("Assets Manager Cleanup");

	if (PHYSFS_deinit() == 0)
		LOG("ERROR DEINITIALIZING THE PHYSFS LIBRARY: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));

	return true;
}