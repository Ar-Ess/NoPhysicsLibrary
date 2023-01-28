#include "Player.h"

Player::Player(DynamicBody* body, Input* input)
{
	this->body = body;
	this->input = input;
}

Player::~Player()
{
	// No need to delete body, physics does it with cleanup
	body = nullptr;
}

bool Player::Update(float dt)
{
	bool ground = body->IsBodyStill(BodyState::ON_GROUND);
	bool shift = (input->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_REPEAT);

	// Inputs
	if (input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
	{
		body->ApplyMomentum(20, 0);
		if (ground && shift) body->ApplyForce(30, 0);
	}

	if (input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
	{
		body->ApplyMomentum(-20, 0);
		if (ground && shift) body->ApplyForce(-30, 0);
	}

	if (ground && input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN)
	{
		float plus = 0;
		if (shift && body->IsBodyStill(BodyState::MOVING))
		{
			plus = -65;
		}
		body->ApplyMomentum(0, -400 + plus);
	}

	return true;
}
