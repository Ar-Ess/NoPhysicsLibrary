#include "Input.h"

#include "SDL/include/SDL.h"

#define MAX_KEYS 300
#define MAX_GAMEPADS 2
#define NUM_CONTROLLER_BUTTONS 15

Input::Input(Window* window) : Module()
{
	keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(mouseButtons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);

	controllerButtons = new KeyState[NUM_CONTROLLER_BUTTONS];
	memset(controllerButtons, KEY_IDLE, sizeof(KeyState) * NUM_CONTROLLER_BUTTONS);

	pad.haptic = nullptr;
	pad.sdlController = nullptr;
	pad.enabled = false;
	pad.index = 0;

	this->window = window;
}

Input::~Input()
{
	delete[] keyboard;
	delete[] controllerButtons;
	delete[] mouseButtons;
}

bool Input::Start()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}	
	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
	{
		LOG("SDL_INIT_GAMECONTROLLER could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	if (SDL_InitSubSystem(SDL_INIT_HAPTIC) < 0)
	{
		LOG("SDL_INIT_HAPTIC could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	SDL_StopTextInput();

	return ret;
}

bool Input::PreUpdate(float dt)
{
	SDL_PumpEvents();
	static SDL_Event event;

	const uint8_t* prevFrameButtons = UpdateGamepadsInput();
	for (int i = 0; i < NUM_CONTROLLER_BUTTONS; ++i)
	{
		if (prevFrameButtons[i] == 1)
		{
			ControllerPressedStateLogic(i);
		}
		else
		{
			ControllerReleasedStateLogic(i);
		}
	}

	UpdateKeyboardInput();
	UpdateMouseInput();

	while(SDL_PollEvent(&event) != 0)
	{
		switch(event.type)
		{
			case SDL_QUIT:
				windowEvents[WE_QUIT] = true;
			break;

			case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
					//case SDL_WINDOWEVENT_LEAVE:
				case SDL_WINDOWEVENT_HIDDEN:
				case SDL_WINDOWEVENT_MINIMIZED:
					break;
				case SDL_WINDOWEVENT_FOCUS_LOST:
					windowEvents[WE_HIDE] = true;
					break;

					//case SDL_WINDOWEVENT_ENTER:
				case SDL_WINDOWEVENT_SHOWN:
				case SDL_WINDOWEVENT_FOCUS_GAINED:
				case SDL_WINDOWEVENT_MAXIMIZED:
					break;
				case SDL_WINDOWEVENT_RESTORED:
					windowEvents[WE_SHOW] = true;
					break;
				}
				break;

			case(SDL_CONTROLLERDEVICEADDED):
				HandleDeviceConnection(event.cdevice.which);
				break;

			case(SDL_CONTROLLERDEVICEREMOVED):
				HandleDeviceRemoval(event.cdevice.which);
				break;

			case SDL_MOUSEBUTTONDOWN:
				mouseButtons[event.button.button - 1] = KEY_DOWN;
				//LOG("Mouse button %d down", event.button.button-1);
				break;

			case SDL_MOUSEBUTTONUP:
				mouseButtons[event.button.button - 1] = KEY_UP;
				//LOG("Mouse button %d up", event.button.button-1);
				break;

			case SDL_MOUSEMOTION:
				int scale = window->GetScale();
				mouseMotion.x = event.motion.xrel / scale;
				mouseMotion.y = event.motion.yrel / scale;
				mouse.x = event.motion.x / scale;
				mouse.y = event.motion.y / scale;
				//LOG("Mouse motion x %d y %d", mouse_motion_x, mouse_motion_y);
				break;
		}
	}
	return true;
}

void Input::UpdateKeyboardInput()
{
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1) // BUTTON PRESSED
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else // BUTTON RELEASED
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}
}

void Input::UpdateMouseInput()
{
	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouseButtons[i] == KEY_DOWN)
			mouseButtons[i] = KEY_REPEAT;

		if (mouseButtons[i] == KEY_UP)
			mouseButtons[i] = KEY_IDLE;
	}
}

uint8_t* Input::UpdateGamepadsInput()
{
	// Iterate through all active gamepads and update all input data
	static uint8_t buttons[NUM_CONTROLLER_BUTTONS] = {};
	// Update all input data
	if (pad.enabled == true)
	{
		buttons[A] = SDL_GameControllerGetButton(pad.sdlController, SDL_CONTROLLER_BUTTON_A) == 1;
		buttons[B] = SDL_GameControllerGetButton(pad.sdlController, SDL_CONTROLLER_BUTTON_B) == 1;
		buttons[X] = SDL_GameControllerGetButton(pad.sdlController, SDL_CONTROLLER_BUTTON_X) == 1;
		buttons[Y] = SDL_GameControllerGetButton(pad.sdlController, SDL_CONTROLLER_BUTTON_Y) == 1;
		buttons[UP_PAD] = SDL_GameControllerGetButton(pad.sdlController, SDL_CONTROLLER_BUTTON_DPAD_UP) == 1;
		buttons[DOWN_PAD] = SDL_GameControllerGetButton(pad.sdlController, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == 1;
		buttons[LEFT_PAD] = SDL_GameControllerGetButton(pad.sdlController, SDL_CONTROLLER_BUTTON_DPAD_LEFT) == 1;
		buttons[RIGHT_PAD] = SDL_GameControllerGetButton(pad.sdlController, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == 1;
		buttons[L1] = SDL_GameControllerGetButton(pad.sdlController, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) == 1;
		buttons[R1] = SDL_GameControllerGetButton(pad.sdlController, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) == 1;
		buttons[L3] = SDL_GameControllerGetButton(pad.sdlController, SDL_CONTROLLER_BUTTON_LEFTSTICK) == 1;
		buttons[R3] = SDL_GameControllerGetButton(pad.sdlController, SDL_CONTROLLER_BUTTON_RIGHTSTICK) == 1;

		buttons[START] = SDL_GameControllerGetButton(pad.sdlController, SDL_CONTROLLER_BUTTON_START) == 1;
		buttons[GUIDE] = SDL_GameControllerGetButton(pad.sdlController, SDL_CONTROLLER_BUTTON_GUIDE) == 1;
		buttons[BACK] = SDL_GameControllerGetButton(pad.sdlController, SDL_CONTROLLER_BUTTON_BACK) == 1;

		if (pad.rumbleCountdown > 0)
			pad.rumbleCountdown--;

		// --- Other buttons that are not in used for the game (provisional) ---
		pad.l2 = float(SDL_GameControllerGetAxis(pad.sdlController, SDL_CONTROLLER_AXIS_TRIGGERLEFT)) / 32767.0f;
		pad.r2 = float(SDL_GameControllerGetAxis(pad.sdlController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT)) / 32767.0f;

		pad.leftX = float(SDL_GameControllerGetAxis(pad.sdlController, SDL_CONTROLLER_AXIS_LEFTX)) / 32767.0f;
		pad.leftY = float(SDL_GameControllerGetAxis(pad.sdlController, SDL_CONTROLLER_AXIS_LEFTY)) / 32767.0f;
		pad.rightX = float(SDL_GameControllerGetAxis(pad.sdlController, SDL_CONTROLLER_AXIS_RIGHTX)) / 32767.0f;
		pad.rightY = float(SDL_GameControllerGetAxis(pad.sdlController, SDL_CONTROLLER_AXIS_RIGHTY)) / 32767.0f;

		// Apply deadzone. All values below the deadzone will be discarded
		pad.leftX = (fabsf(pad.leftX) >	pad.leftDeadZone) ? pad.leftX : 0.0f;
		pad.leftY = (fabsf(pad.leftY) >	pad.leftDeadZone) ? pad.leftY : 0.0f;
		pad.rightX = (fabsf(pad.rightX) > pad.rightDeadZone) ? pad.rightX : 0.0f;
		pad.rightY = (fabsf(pad.rightY) > pad.rightDeadZone) ? pad.rightY : 0.0f;
	}
	return buttons;
}

void Input::ControllerPressedStateLogic(int id)
{
	if (controllerButtons[id] == KEY_IDLE)
		controllerButtons[id] = KEY_DOWN;
	else
		controllerButtons[id] = KEY_REPEAT;
}

void Input::ControllerReleasedStateLogic(int id)
{
	if (controllerButtons[id] == KEY_REPEAT || controllerButtons[id] == KEY_DOWN)
		controllerButtons[id] = KEY_UP;
	else
		controllerButtons[id] = KEY_IDLE;
}

bool Input::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	if (pad.haptic != nullptr)
	{
		SDL_HapticStopAll(pad.haptic);
		SDL_HapticClose(pad.haptic);
	}
	if (pad.sdlController != nullptr && pad.enabled == true) SDL_GameControllerClose(pad.sdlController);

	SDL_QuitSubSystem(SDL_INIT_HAPTIC);
	SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	return true;
}

bool Input::GetWindowEvent(EventWindow ev)
{
	return windowEvents[ev];
}

void Input::GetMousePosition(float& x, float& y)
{
	x = mouse.x;
	y = mouse.y;
}

Point Input::GetMousePosition() const
{
	return mouse;
}

void Input::GetMouseMotion(float& x, float& y)
{
	x = mouseMotion.x;
	y = mouseMotion.y;
}

Point Input::GetMouseMotion() const
{
	return mouseMotion;
}

// CONTROLLER
void Input::HandleDeviceConnection(int index)
{
	if (SDL_IsGameController(index))
	{
		if (pad.enabled == false)
		{
			if (pad.sdlController = SDL_GameControllerOpen(index))
			{
				LOG("Found a gamepad with name %s", SDL_GameControllerName(pad.sdlController));
				pad.enabled = true;
				pad.leftDeadZone = pad.rightDeadZone = 0.1f;
				pad.haptic = SDL_HapticOpen(index);
				if (pad.haptic != nullptr)
					LOG("... gamepad has force feedback capabilities");
				pad.index = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(pad.sdlController));
			}
		}	
	}
}

void Input::HandleDeviceRemoval(int index)
{
	// If an existing gamepad has the given index, deactivate all SDL device functionallity
	if (pad.enabled && pad.index == index)
	{
		pad.enabled = false;
		SDL_HapticClose(pad.haptic);
		SDL_GameControllerClose(pad.sdlController);
		memset(&pad, 0, sizeof(GamePad));
	}	
}

bool Input::ShakeController(int id, int duration, float strength)
{
	bool ret = false;

	// Check if the given id is valid within the array bounds
	if (id < 0 || id >= MAX_PADS) return ret;

	// Check if the gamepad is active and allows rumble
	if (!pad.enabled || pad.haptic == nullptr || SDL_HapticRumbleSupported(pad.haptic) != SDL_TRUE) return ret;

	// If the pad is already in rumble state and the new strength is below the current value, ignore this call
	if (duration < pad.rumbleCountdown && strength < pad.rumbleStrength)
		return ret;

	if (SDL_HapticRumbleInit(pad.haptic) == -1)
	{
		LOG("Cannot init rumble for controller number %d", id);
	}
	else
	{
		SDL_HapticRumbleStop(pad.haptic);
		SDL_HapticRumblePlay(pad.haptic, strength, duration / 60 * 1000); //Conversion from frames to ms at 60FPS

		pad.rumbleCountdown = duration;
		pad.rumbleStrength = strength;

		ret = true;
	}

	return ret;
}

const char* Input::GetControllerName(int id) const
{
	// Check if the given id has a valid controller
	if (pad.enabled == true && pad.sdlController != nullptr)
		return SDL_GameControllerName(pad.sdlController);

	return "unplugged";
}