#include "App.h"

#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL.h"
#include <stdlib.h>

enum class MainState
{
	CREATE = 1,
	START,
	LOOP,
	CLEAN,
	FAIL,
	EXIT
};

App* app = nullptr;

int main(int argc, char* args[])
{
	LOG("Engine starting ...");

	MainState state = MainState::CREATE;
	int result = EXIT_FAILURE;

	while(state != MainState::EXIT)
	{
		switch(state)
		{
			// Allocate the engine --------------------------------------------
			case MainState::CREATE:
			LOG("CREATION PHASE ===============================");

			app = new App(argc, args);

			if(app != NULL)
				state = MainState::START;
			else
				state = MainState::FAIL;

			break;

			// Call all modules before first frame  ----------------------------
			case MainState::START:
			LOG("START PHASE ===============================");
			if(app->Start() == true)
			{
				state = MainState::LOOP;
				LOG("UPDATE PHASE ===============================");
			}
			else
			{
				state = MainState::FAIL;
				LOG("ERROR: Start failed");
			}
			break;

			// Loop all modules until we are asked to leave ---------------------
			case MainState::LOOP:
			if(app->Update() == false)
				state = MainState::CLEAN;
			break;

			// Cleanup allocated memory -----------------------------------------
			case MainState::CLEAN:
			LOG("CLEANUP PHASE ===============================");
			if(app->CleanUp() == true)
			{
				RELEASE(app);
				result = EXIT_SUCCESS;
				state = MainState::EXIT;
			}
			else
				state = MainState::FAIL;

			break;

			// Exit with errors and shame ---------------------------------------
			case MainState::FAIL:
			LOG("Exiting with errors :(");
			result = EXIT_FAILURE;
			state = MainState::EXIT;
			break;
		}
	}

	// Dump memory leaks
	return result;
}