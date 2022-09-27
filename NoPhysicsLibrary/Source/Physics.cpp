#include "Physics.h"
#include "LiquidBody.h"
#include "DynamicBody.h"

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
	DetectCollisions(bodies);

	SolveCollisions();
}

void Physics::CleanUp()
{
	for (Collision* c : collisions) RELEASE(c);
	collisions.clear();
}

void Physics::UpdateDynamic(float dt, Body* b)
{
	DynamicBody* body = (DynamicBody*)b;
	//AutoApplyForces(); // Future

	// Multiplying global gravity * mass to acquire the force (Global gravity falls :) )
	body->ApplyForce(globalGravity.x * body->GetMass(), globalGravity.y * body->GetMass());

	// Multiplying intrinsic gravity * mass to acquire the force (Local gravity falls :) )
	body->ApplyForce(body->gravityOffset.x * body->GetMass(), body->gravityOffset.y * body->GetMass());

	// Second law newton
	body->SecondNewton(); // Suma de forces a acceleració

	// First law Buxeda
	body->FirstBuxeda(); // Suma de momentum a velocity

	body->Backup();

	// Integrate
	Integrate(dt, b);

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

void Physics::DetectCollisions(std::vector<Body*>* bodies)
{
	//-Todone: This is optimized, adapt it for declip function
	size_t size = bodies->size();
	for (unsigned int i = 0; i < size - 1; ++i)
	{
		Body* b1 = bodies->at(i);
		if (b1->GetClass() == BodyClass::GAS_BODY || b1->GetClass() == BodyClass::LIQUID_BODY) continue;

		for (unsigned int a = 1 + i; a < size; ++a)
		{
			Body* b2 = bodies->at(a);
			if (b2->GetClass() == BodyClass::GAS_BODY || b2->GetClass() == BodyClass::LIQUID_BODY) continue;
			if (b1->GetClass() == BodyClass::STATIC_BODY || b2->GetClass() == BodyClass::STATIC_BODY) continue;

			Rect intersect = MathUtils::IntersectRectangle(b1->GetRect(), b2->GetRect());
			if (intersect.IsNull()) continue;

			Body* dyn = b2;
			Body* other = b2;
			b1->GetClass() == BodyClass::DYNAMIC_BODY ? dyn = b1 : other = b1;

			collisions.emplace_back(new Collision(dyn, other, intersect));
		}
	}
}

void Physics::SolveCollisions()
{
	for (Collision* c : collisions)
	{
		DynamicBody* dynamicBody = (DynamicBody*)c->dynamicBody;

		Point dDeltaPosVec = dynamicBody->GetPosition() - dynamicBody->backup.position;
		switch (c->body->GetClass())
		{
		case BodyClass::DYNAMIC_BODY:
			Point bDeltaPosVec = c->body->GetPosition();
			break;

		case BodyClass::STATIC_BODY:

			break;

		default:
			break;
		}

		RELEASE(c);
	}
	collisions.clear();
}