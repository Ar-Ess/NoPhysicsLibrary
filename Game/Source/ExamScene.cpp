#include "ExamScene.h"
#include "Vector.h"

ExamScene::ExamScene()
{
}

ExamScene::~ExamScene()
{
}

bool ExamScene::Start()
{
	//-Exercice 2: Set your library variable with 128 pixls/meter, set a physics and scenario preset of your choice. (Windows Size can be accessed from "render->GetSize()")

	//-Exercice 4: Configure the library. 
	// -> Enable collisions debugging
	// -> The pan range to 10
	// -> Set the player as listener
	// -> Load this audio file ("Assets/Audio/bounce.wav")

	//-Exercice 5: Create the bodies for your world.
	// -> Create a body for the player, it has an average weight
	// -> Create an obstacle which will emmit sound
	// -> Create a liquid body with w: 2,6 meters and h: 2,1 meters. Configure the liquid to be water with maximum buoyancy
	// -> Create a gas body similar than air with a density of 1500kg/m^2, 1000pa of pressure, 1.5 x drag amd 0.1 y drag
	//			-> The size of this body must be as big as all the scenario creator

	render->ResetCamera();

	return true;
}

bool ExamScene::Update(float dt)
{
	static bool pause = false;

	//-Exercice 6: Create platformer movement for the player variable (for input conditionals, use "input->GetKey(SDL_SCANCODE_...) == KeyState::KEY_...")
	// -> Move right
	// -> Move left
	// -> Jump (only when touching the ground)
	// -> Water Lateral Impulse (only when being on a liquid)
	// -> Reset all player forces with "R"
	// -> Pause physics with "Esc" (use the pause variable setted)
	// -> Make your obstacle emit a sound with "Return"

	//-Exercice 7: Update the library and draw the following
	// -> In the first for loop, draw the bodies and emission points (make it a bigger rect) of them using "render->DrawRectangle(rect, color)"
	// -> In the second for loop, draw the debug collisions

	// First for loop
	unsigned int size = 0;
	for (int i = 0; i < size; ++i)
	{
		//BodyClass eraseThis;
		SDL_Color color = { 0, 0, 0, 50 };

		//switch (eraseThis)
		//{
		//case BodyClass::STATIC_BODY:  color = { 255,   0,   0, color.a }; break;
		//case BodyClass::DYNAMIC_BODY: color = { 0, 255,   0, color.a }; break;
		//case BodyClass::LIQUID_BODY:  color = { 100, 100, 255, color.a }; break;
		//case BodyClass::GAS_BODY:     color = { 255, 255, 255, (Uint8)(color.a - 20) }; break;
		//}

	}

	// Second for loop
	size = 0;
	for (int i = 0; i < size; ++i)
	{
	}

	//Change scene
	if (input->GetKey(SDL_SCANCODE_BACKSPACE) == KeyState::KEY_DOWN) SetScene(Scenes::INITIAL_SCENE);

	return true;
}

bool ExamScene::CleanUp()
{
	//-Exercice 3: If your library variable is setted, clear it and then, release it with "RELEASE()" function
	// -> Set all the other variables to nullptr
	return true;
}
