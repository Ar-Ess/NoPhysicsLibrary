#include "Physics.h"
#include "LiquidBody.h"
#include "DynamicBody.h"

#include "GhostSlot.h"
#include "DynArray.h"
#include "MathUtils.h"

Physics::Physics()
{
}

Physics::~Physics()
{}

void Physics::Step(Body* body, float dt)
{
	switch (body->GetClass())
	{
	case BodyClass::DYNAMIC_BODY: UpdateDynamic(dt, body); break;
	case BodyClass::LIQUID_BODY : UpdateLiquid (dt, body); break;
	}
}

void Physics::Declip(std::vector<Body*>* bodies)
{
	//-Todo: This is optimized, adapt it for declip function
	size_t size = bodies->size();
	for (uint i = 0; i < size - 1; ++i)
	{
		for (uint a = 1 + i; a < size; ++a)
		{

		}
	}

	// Declip Iteration 1
	/*std::vector<Body*>::const_iterator it1;
	std::vector<Body*>::const_iterator it2;
	size_t size = bodies->size();
	int offset = 0;
	int iter1 = -1;
	for (it1 = bodies->begin() + offset; it1 != bodies->end(); ++it1)
	{
		++iter1;
		Body* b1 = *it1;
		if (b1->GetClass() == BodyClass::GAS_BODY || b1->GetClass() == BodyClass::LIQUID_BODY) continue;
		int iter2 = -1;

		for (it2 = bodies->begin() + 1 + offset; it2 != bodies->end(); ++it2)
		{
			++iter2;
			Body* b2 = *it2;

			if (b2->GetClass() == BodyClass::GAS_BODY || b2->GetClass() == BodyClass::LIQUID_BODY) continue;
			if (b1->GetClass() == BodyClass::STATIC_BODY && b2->GetClass() == BodyClass::STATIC_BODY) continue;

			Rect intersectRect = MathUtils::IntersectRectangle(b1->GetRect(), b2->GetRect());
			if (intersectRect.IsNull()) continue;

			collisions.emplace_back(new Collision(iter1, iter2, intersectRect));
		}

		++offset;
	}

	collisions.clear();*/

}

void Physics::CleanUp()
{
}

void Physics::UpdateDynamic(float dt, Body* b)
{
	DynamicBody* body = (DynamicBody*)b;
	AutoApplyForces(); // Future

	// Multiplying global gravity * mass to acquire the force (Global gravity falls :) )
	body->ApplyForce(globalGravity.x * body->GetMass(), globalGravity.y * body->GetMass());

	// Multiplying intrinsic gravity * mass to acquire the force (Local gravity falls :) )
	body->ApplyForce(body->gravityOffset.x * body->GetMass(), body->gravityOffset.y * body->GetMass());

	// Second law newton
	body->SecondNewton(); // Suma de forces a acceleració

	// First law Buxeda
	body->FirstBuxeda(); // Suma de momentum a velocity

	BodyBackup backup = body->Backup();

	// Integrate
	Integrate(dt, b);

	// Check Collisions
	//CheckCollisions(body, backup);
}

void Physics::UpdateLiquid(float dt, Body* b)
{
	LiquidBody* body = (LiquidBody*)b;
}

void Physics::Integrate(float dt, Body* b)
{
	DynamicBody* body = (DynamicBody*)b;
	// Second Order Red Velvet Integrator

	body->rect.x += body->velocity.x * dt + 0.5f * body->acceleration.x * dt * dt;
	body->velocity.x += body->acceleration.x * dt;

	body->rect.y += body->velocity.y * dt + 0.5f * body->acceleration.y * dt * dt;
	body->velocity.y += body->acceleration.y * dt;
}

void Physics::AutoApplyForces()
{
	// -TODO: Iteration check collision with environment

	AutoApplyHydroDrag();
	AutoApplyHydroLift();
	AutoApplyAeroDrag();
	AutoApplyAeroLift();
	AutoApplyBuoyancy();
}

void Physics::AutoApplyAeroDrag()
{
}

void Physics::AutoApplyAeroLift()
{
}

void Physics::AutoApplyHydroDrag()
{
}

void Physics::AutoApplyHydroLift()
{
}

void Physics::AutoApplyBuoyancy()
{
}

int Physics::DirectionDetection(Point currPos, Point prevPos)
{
	if (prevPos.y - currPos.y < 0) // DOWN
	{
		return 1; //COLLIDING UP TO DOWN 
	}
	if (prevPos.x - currPos.x < 0) // RIGHT
	{
		return 3; //COLLIDING LEFT TO RIGHT
	}
	if (prevPos.y - currPos.y > 0) // UP
	{
		return 0; //COLLIDING DOWN TO UP
	}
	if (prevPos.x - currPos.x > 0) // LEFT
	{
		return 2; //COLLIDING RIGHT TO LEFT
	}

	return -1;
}

int Physics::InvertDirection(int dir)
{
	Direction direction = (Direction)dir;
	switch (direction)
	{
	case Direction::UP: return 1; break;
	case Direction::DOWN: return 0; break;
	case Direction::LEFT: return 3; break;
	case Direction::RIGHT: return 2; break;
	}

	return -1;
}