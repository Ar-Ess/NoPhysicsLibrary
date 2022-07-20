#include "NoPhysicsLibrary.h"
#include "Physics.h"

static Physics* physics = nullptr;

void NPL::Init()
{
	// You've alreay initialized the library once
	assert(physics == nullptr);

	physics = new Physics();
}

BodyCreation NPL::CreateBody(Rect rectangle, float mass)
{
	//Library not initialized. Call NPL::Init() first
	assert(physics != nullptr);

	return BodyCreation(rectangle, mass, &physics->bodies);
}

bool NPL::Update(float dt)
{
	return false;
}
