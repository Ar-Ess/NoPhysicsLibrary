#include "Input.h"
#include "SDL/include/SDL.h"

#define MAX_KEYS 300
#define MAX_GAMEPADS 2
#define NUM_CONTROLLER_BUTTONS 15

Input::Input(Window* window) : Module()
{
	keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, (int)KeyState::KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	mouseButtons = new KeyState[NUM_MOUSE_BUTTONS];
	memset(mouseButtons, (int)KeyState::KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);

	this->window = window;
}

Input::~Input()
{
	delete[] keyboard;
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

	return ret;
}

bool Input::PreUpdate(float dt)
{
	SDL_PumpEvents();
	static SDL_Event event;

	UpdateKeyboardInput();
	UpdateMouseInput();

	while(SDL_PollEvent(&event) != 0)
	{
		switch(event.type)
		{
			case SDL_QUIT:
				windowEvents[(int)EventWindow::WE_QUIT] = true;
			break;

			case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
					//case SDL_WINDOWEVENT_LEAVE:
				case SDL_WINDOWEVENT_HIDDEN:
				case SDL_WINDOWEVENT_MINIMIZED:
					break;
				case SDL_WINDOWEVENT_FOCUS_LOST:
					windowEvents[(int)EventWindow::WE_HIDE] = true;
					break;

					//case SDL_WINDOWEVENT_ENTER:
				case SDL_WINDOWEVENT_SHOWN:
				case SDL_WINDOWEVENT_FOCUS_GAINED:
				case SDL_WINDOWEVENT_MAXIMIZED:
					break;
				case SDL_WINDOWEVENT_RESTORED:
					windowEvents[(int)EventWindow::WE_SHOW] = true;
					break;
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				mouseButtons[event.button.button - 1] = KeyState::KEY_DOWN;
				//LOG("Mouse button %d down", event.button.button-1);
				break;

			case SDL_MOUSEBUTTONUP:
				mouseButtons[event.button.button - 1] = KeyState::KEY_UP;
				//LOG("Mouse button %d up", event.button.button-1);
				break;

			case SDL_MOUSEMOTION:
				int scale = window->GetScale();
				mouseMotion.x = float(event.motion.xrel) / float(scale);
				mouseMotion.y = float(event.motion.yrel) / float(scale);
				mouse.x = float(event.motion.x) / float(scale);
				mouse.y = float(event.motion.y) / float(scale);
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
			if (keyboard[i] == KeyState::KEY_IDLE)
				keyboard[i] = KeyState::KEY_DOWN;
			else
				keyboard[i] = KeyState::KEY_REPEAT;
		}
		else // BUTTON RELEASED
		{
			if (keyboard[i] == KeyState::KEY_REPEAT || keyboard[i] == KeyState::KEY_DOWN)
				keyboard[i] = KeyState::KEY_UP;
			else
				keyboard[i] = KeyState::KEY_IDLE;
		}
	}
}

void Input::UpdateMouseInput()
{
	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouseButtons[i] == KeyState::KEY_DOWN)
			mouseButtons[i] = KeyState::KEY_REPEAT;

		if (mouseButtons[i] == KeyState::KEY_UP)
			mouseButtons[i] = KeyState::KEY_IDLE;
	}
}

bool Input::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	return true;
}

bool Input::GetWindowEvent(EventWindow ev)
{
	return windowEvents[(int)ev];
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