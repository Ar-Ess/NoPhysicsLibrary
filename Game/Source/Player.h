#pragma once
#include "NoPhysicsLibrary.h"
#include "Input.h"

class Player
{
public:

	Player(DynamicBody* body, Input* input);

	~Player();

	bool Update(float dt);

	DynamicBody* Body() const
	{
		return body;
	}

	Point Position() const
	{
		return body->GetPosition(InUnit::IN_PIXELS);
	}

private:

	DynamicBody* body = nullptr;
	Input* input = nullptr;

};
