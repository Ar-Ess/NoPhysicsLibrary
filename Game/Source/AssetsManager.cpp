#include "AssetsManager.h"
#include "App.h"

#include "Defs.h"
#include "Log.h"

#include "physfsrwops.h"
#include "physfsrwops.c"

AssetsManager::AssetsManager() : Module()
{
}

// Destructor
AssetsManager::~AssetsManager()
{
}

// Called before Assets Manager is available
bool AssetsManager::Start()
{
	LOG("Loading Assets Manager");
	bool ret = true;

	// (SOLVED) TODO 0: Initialize the PhysFS API and mount the Assets file, return false to check if there is any error

	if (PHYSFS_init(NULL) == 0)
	{
		LOG("ERROR INITIALIZING PHYSFS LIBRARY: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return false;
	}

	if (PHYSFS_mount("Assets.pak", NULL, 1) == 0)
	{
		LOG("ERROR ADDING ARCHIVE TO SEARCH PATH: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return false;
	}

	return ret;
}

SDL_RWops* AssetsManager::LoadAsset(const char* path)
{
	// (SOLVED) TODO 1: Check if the file intended to load actually exists in the Assets ZIP
	if (PHYSFS_exists(path) == 0)
	{
		LOG("ERROR - FILE %s DOESNT EXIST IN THE SEARCH PATH: %s\n", path, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return NULL;
	}

	// (SOLVED) TODO 2: Open the file for reading using the RWops accessible structure by PhysFS, and save that structure for the function to return.
	SDL_RWops* ret = PHYSFSRWOPS_openRead(path);

	if (ret == NULL)
	{
		LOG("ERROR OPENING FILE %s FOR READING: %s\n", path, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return NULL;
	}

	return ret;
}

size_t AssetsManager::LoadXML(const char* path, char** buffer)
{
	// (SOLVED) TODO 3: Repeat what you have done in the LoadAsset() method but instead of using a RWops structure, use a PHYSFS_file
	if (PHYSFS_exists(path) == 0)
		LOG("ERROR - FILE %s DOESNT EXIST IN THE SEARCH PATH: %s\n", path, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));

	PHYSFS_file* file = PHYSFS_openRead(path);
	if (file == NULL)
		LOG("ERROR OPENING FILE %s FOR READING: %s\n", path, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));

	// (SOLVED) TODO 4: Check if PhysFS has not ended to read the file and obtain the size of the file in bytes
	if (!PHYSFS_eof(file))
	{
		PHYSFS_sint64 size = PHYSFS_fileLength(file);

		// (SOLVED) TODO 5: Allocate enough memory for the buffer to read the file (Be aware to modify the contents of the buffer)
		*buffer = new char[size];

		PHYSFS_sint64 numBytesRead = PHYSFS_readBytes(file, *buffer, size);
		if (numBytesRead == -1)
			LOG("ERROR READING FROM FILEHANDLE: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));

		// (SOLVED) TODO 6: Close the file when finished and return its number of bytes
		// If the reading process is successful (has finished) it means that the number of byes read is equal to the size of the file.
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

// Called before quitting
bool AssetsManager::Cleanup()
{
	LOG("Quitting Assets Manager");

	if (PHYSFS_deinit() == 0)
		LOG("ERROR DEINITIALIZING THE PHYSFS LIBRARY: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));

	return true;
}