#ifndef __INPUT_H__
#define __INPUT_H__

#include "Module.h"
#include "Window.h"

#include "Point.h"

//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
#define MAX_PADS 1
//#define LAST_KEYS_PRESSED_BUFFER 50

struct SDL_Rect;
struct _SDL_GameController;
struct _SDL_Haptic;

enum class EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum class KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

enum class GamePadInput
{
	A = 0,
	B,
	X,
	Y,
	UP_PAD,
	DOWN_PAD,
	LEFT_PAD,
	RIGHT_PAD,
	L1,
	R1,
	L3,
	R3,
	START,
	GUIDE,
	BACK,
};

struct GamePad
{
	//Input data
	//bool start, back, guide;
	//bool x, y, a, b, l1, r1, l3, r3;
	//bool up, down, left, right;
	float l2 = 0.0f, r2 = 0.0f;
	float leftX = 0.0f, leftY = 0.0f, rightX = 0.0f, rightY = 0.0f, leftDeadZone = 0.0f, rightDeadZone = 0.0f;

	//Controller data
	bool enabled = false;
	int index = 0;
	_SDL_GameController* sdlController = nullptr;
	_SDL_Haptic* haptic = nullptr;

	//Rumble controller
	int rumbleCountdown = 0;
	float rumbleStrength = 0.0f;
};

class Input : public Module
{
public:

	Input(Window* window);

	virtual ~Input();

	bool Start();

	bool PreUpdate(float dt);

	void UpdateKeyboardInput();

	void UpdateMouseInput();

	uint8_t* UpdateGamepadsInput();

	bool CleanUp();

	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	KeyState GetControl(int id) const
	{
		return controllerButtons[id];
	}

	KeyState GetMouseButtonDown(int id) const
	{
		return mouseButtons[id - 1];
	}

	bool GetPadEnabled() const
	{
		return pad.enabled;
	}

	void ControllerPressedStateLogic(int id);

	void ControllerReleasedStateLogic(int id);

	bool GetWindowEvent(EventWindow ev);

	void GetMousePosition(float &x, float &y);

	Point GetMousePosition() const;

	void GetMouseMotion(float& x, float& y);

	Point GetMouseMotion() const;

	void HandleDeviceConnection(int index);

	void HandleDeviceRemoval(int index);

	bool ShakeController(int id, int duration, float strength = 0.5f);

	const char* GetControllerName(int id) const;

private:

	Window* window = nullptr;

	bool windowEvents[(int)EventWindow::WE_COUNT];

	KeyState* keyboard = nullptr;	
	KeyState* mouseButtons = nullptr;

	GamePad pad;
	KeyState* controllerButtons = nullptr;
		
	Point mouseMotion = {};
	Point mouse = {};
};

#endif // __INPUT_H__