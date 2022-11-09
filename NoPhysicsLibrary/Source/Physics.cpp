#include "Physics.h"
#include "LiquidBody.h"
#include "GasBody.h"
#include "DynamicBody.h"
#include "StaticBody.h"

#include "MathUtils.h"
#include "Ray.h"

Physics::Physics(const std::vector<Body*>* bodies, const Flag* physicsConfig, const std::vector<unsigned int*>* gasIndex, const std::vector<unsigned int*>* liquidIndex, const float* pixelsToMeters)
{
	this->bodies = bodies;
	this->physicsConfig = physicsConfig;
	this->gasIndex = gasIndex;
	this->liquidIndex = liquidIndex;
	this->pixelsToMeters = pixelsToMeters;
}

Physics::~Physics()
{}

void Physics::Step(std::vector<Body*>* bodies, float dt)
{
	for (Body* b : *bodies)
	{
		switch (b->GetClass())
		{
		case BodyClass::DYNAMIC_BODY: UpdateDynamic(dt, b); break;
		case BodyClass::LIQUID_BODY:  UpdateLiquid( dt, b); break;
		}
	}
}

void Physics::SolveCollisions(std::vector<Body*>* bodies)
{
	ResetFlags(bodies);

	DetectCollisions(bodies);

	Declip();
}

void Physics::CleanUp()
{
	for (Collision* c : collisions) RELEASE(c);
	collisions.clear();
}

void Physics::UpdateDynamic(float dt, Body* b)
{
	DynamicBody* body = (DynamicBody*)b;
	
	AutoApplyForces(body); //-TODO: Future

	// Multiplying global gravity * mass to acquire the force (Global gravity falls :) )
	body->ApplyForce(globalGravity, InUnit::IN_METERS);

	// Multiplying intrinsic gravity * mass to acquire the force (Local gravity falls :) )
	body->ApplyForce(body->gravityOffset, InUnit::IN_METERS);

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

void Physics::AutoApplyForces(DynamicBody* body)
{
	// -TODO: Iteration check collision with environment

	//AutoApplyHydroDrag(body);

	//AutoApplyHydroLift(body);

	AutoApplyAeroDrag(body);

	//AutoApplyAeroLift(body);

	//AutoApplyBuoyancy(body);
}

void Physics::AutoApplyAeroDrag(DynamicBody* body)
{
	if (!body->IsColliding(BodyState::FLOAT) && body->IsColliding(BodyState::GAS)) return;

	const float fullArea = body->GetRect(InUnit::IN_METERS).GetArea();
	float areaCovered = 0;
	for (unsigned int* i : *gasIndex)
	{
		GasBody* gas = (GasBody*)bodies->at(*i);
		if (!MathUtils::CheckCollision(gas->GetRect(InUnit::IN_METERS), body->GetRect(InUnit::IN_METERS))) continue;
		float area = MathUtils::IntersectRectangle(bodies->at(*i)->GetRect(InUnit::IN_METERS), body->GetRect(InUnit::IN_METERS)).GetArea();
		areaCovered += area;
		//-TODO: Check whether we need to multiply the mass inside AddForce function or create an "AddForceInternal" that avoids multiplying by the mass
		//-TODO: check if this is actually working
		int neg = -1;
		if (body->velocity.x < 0) neg = 1;
		float aerodragX = neg * 0.5 * gas->GetDensity() * MathUtils::Pow(body->velocity.x, 2) * area * gas->GetDragCoefficient().x;
		float aerodragY = neg * 0.5 * gas->GetDensity() * MathUtils::Pow(body->velocity.y, 2) * area * gas->GetDragCoefficient().y;
		body->forces.emplace_back(new Force(aerodragX, aerodragY, InUnit::IN_METERS));
		if (areaCovered - fullArea < 0.0001f) break;
	}
}

void Physics::AutoApplyAeroLift(DynamicBody* body)
{
}

void Physics::AutoApplyHydroDrag(DynamicBody* body)
{
	if (!body->IsColliding(BodyState::LIQUID)) return;

	const float fullArea = body->GetRect(InUnit::IN_METERS).GetArea();
	float areaCovered = 0;
	for (unsigned int* i : *liquidIndex)
	{
		LiquidBody* liquid = (LiquidBody*)bodies->at(*i);
		if (!MathUtils::CheckCollision(liquid->GetRect(InUnit::IN_METERS), body->GetRect(InUnit::IN_METERS))) continue;
		float area = MathUtils::IntersectRectangle(bodies->at(*i)->GetRect(InUnit::IN_METERS), body->GetRect(InUnit::IN_METERS)).GetArea();
		areaCovered += area;
		//-TODO: Check whether we need to multiply the mass inside AddForce function or create an "AddForceInternal" that avoids multiplying by the mass
		//-TODO: check if this is actually working
		int neg = -1;
		if (body->velocity.x < 0) neg = 1;
		float hydrodragX = neg * 0.5 * liquid->GetDensity(InUnit::IN_METERS) * MathUtils::Pow(body->velocity.x, 2) * area /* * liquid->GetDragCoefficient().x*/;
		float hydrodragY = neg * 0.5 * liquid->GetDensity(InUnit::IN_METERS) * MathUtils::Pow(body->velocity.y, 2) * area /* * liquid->GetDragCoefficient().y*/;
		body->forces.emplace_back(new Force(hydrodragX, hydrodragY, InUnit::IN_METERS));
		if (areaCovered - fullArea < 0.0001f) break;
	}
}

void Physics::AutoApplyHydroLift(DynamicBody* body)
{
}

void Physics::AutoApplyBuoyancy(DynamicBody* body)
{
}

void Physics::DetectCollisions(std::vector<Body*>* bodies)
{
	// If collisions debugging is enabled
	if (physicsConfig->Get(0))
	{
		if (!collisions.empty())
		{
			for (Collision* c : collisions) RELEASE(c);

				collisions.clear();
		}
	}

	// New (iterate dynamic bodies with all)
	size_t size = bodies->size();
	for (unsigned int i = 0; i < size - 1; ++i)
	{
		Body* b1 = bodies->at(i);
		if (b1->GetClass() != BodyClass::DYNAMIC_BODY) continue;
		if (!b1->IsCollidable()) continue;

		for (unsigned int a = 0; a < size; ++a)
		{
			if (a == i) continue;
			Body* b2 = bodies->at(a);
			if (b2->GetClass() == BodyClass::GAS_BODY || !b2->IsCollidable()) continue;
			if (b2->GetClass() == BodyClass::DYNAMIC_BODY && a < i) continue;

			Rect intersect = MathUtils::IntersectRectangle(b1->GetRect(InUnit::IN_METERS), b2->GetRect(InUnit::IN_METERS));
			if (!MathUtils::CheckCollision(b1->GetRect(InUnit::IN_METERS), b2->GetRect(InUnit::IN_METERS))) continue;

			collisions.emplace_back(new Collision((DynamicBody*)b1, b2, intersect, pixelsToMeters));
		}
	}
}

void Physics::ResetFlags(std::vector<Body*>* bodies)
{
	for (Body* b : *bodies)
	{
		if (b->GetClass() != BodyClass::DYNAMIC_BODY) continue;

		((DynamicBody*)b)->bodyState.Clear();
	}
}

void Physics::Declip()
{
	for (Collision* c : collisions)
	{
		DynamicBody* dynBody = (DynamicBody*)c->GetDynBody();
		Body* b = (Body*)c->GetBody();

		Rect intersect = c->GetCollisionRectangle(InUnit::IN_METERS);
		Point directionVec = dynBody->GetPosition(InUnit::IN_METERS) - dynBody->backup.position;

		switch (b->GetClass())
		{

		case BodyClass::DYNAMIC_BODY:
		{
			DynamicBody* body = (DynamicBody*)b;
			Point directionVecAlter = body->GetPosition(InUnit::IN_METERS) - body->backup.position;
			Point normal = {};

			Point centerOfIntersecion = c->GetCollisionRectangle(InUnit::IN_METERS).GetPosition(Align::CENTER);
			Ray ray(centerOfIntersecion.Apply(directionVec.Multiply(-1.0f)), centerOfIntersecion);
			if (!MathUtils::RayCast(ray, body->GetRect(InUnit::IN_METERS), normal)) break;

			if (normal.x == 0) // Vertical collision with horizontal surface
			{
				// Top -> Bottom
				if (directionVec.y > 0)
				{
					dynBody->rect.y -= intersect.h / 2;
					body->rect.y += intersect.h / 2;
					dynBody->bodyState.Set((int)BodyState::GROUND, true);
				}
				// Bottom -> Top
				if (directionVec.y < 0)
				{
					dynBody->rect.y += intersect.h / 2;
					body->rect.y -= intersect.h / 2;
					dynBody->bodyState.Set((int)BodyState::ROOF, true);
				}

				// Perfectly elastic collision
				dynBody->velocity.y *= -1;

				// Loss of energy
				dynBody->velocity.y *= MathUtils::Abs(1 - globalRestitution.y);
				dynBody->velocity.x *= MathUtils::Abs(1 - globalFriction.x);

			}
			else if (normal.y == 0) // Horizontal collision with vertical surface
			{
				// Left -> Right
				if (directionVec.x > 0)
				{
					dynBody->rect.x -= intersect.w / 2;
					body->rect.x += intersect.w / 2;
					dynBody->bodyState.Set((int)BodyState::RIGHT, true);
				}
				// Right -> Left
				if (directionVec.x < 0)
				{
					dynBody->rect.x += intersect.w / 2;
					body->rect.x -= intersect.w / 2;
					dynBody->bodyState.Set((int)BodyState::LEFT, true);
				}

				// Perfectly elastic collision
				dynBody->velocity.x *= -1;

				// Loss of energy
				dynBody->velocity.x *= MathUtils::Abs(1 - globalRestitution.x);
				dynBody->velocity.y *= MathUtils::Abs(1 - globalFriction.y);

			}
			else
			{
				Rect wtfRect = c->GetBody()->GetRect(InUnit::IN_METERS);
				Point wtfOrgn = dynBody->backup.position;
				Point wtfDir = directionVec;
				LOG("Weird Rectangle: X = %.f, Y = %.f, Width = %.f, Heigth = %.f\nWeird Ray: X = %.f, Y = %.f, Dir X = %.f, Dir Y = %.f", wtfRect.x, wtfRect.y, wtfRect.w, wtfRect.h, wtfOrgn.x, wtfOrgn.y, wtfDir.x, wtfDir.y);
				assert("This is impossible, how could this happen???? :O Quick, check the log!!!!");
			}

			break;
		}

		case BodyClass::STATIC_BODY:
		{
			StaticBody* body = (StaticBody*)b;
			Point normal = {};

			Point centerOfIntersecion = c->GetCollisionRectangle(InUnit::IN_METERS).GetPosition(Align::CENTER);
			Ray ray(centerOfIntersecion.Apply(directionVec.Multiply(-1.0f)), centerOfIntersecion);
			if (!MathUtils::RayCast(ray, body->GetRect(InUnit::IN_METERS), normal)) break;

			if (normal.x == 0) // Vertical collision with horizontal surface
			{
				// Top -> Bottom
				if (directionVec.y > 0)
				{
					dynBody->rect.y = body->GetPosition(InUnit::IN_METERS).y - dynBody->rect.h;
					dynBody->bodyState.Set((int)BodyState::GROUND, true);
				}
				// Bottom -> Top
				if (directionVec.y < 0)
				{
					dynBody->rect.y = body->GetRect(InUnit::IN_METERS).GetPosition(Align::BOTTOM_CENTER).y;
					dynBody->bodyState.Set((int)BodyState::ROOF, true);
				}

				// Perfectly elastic collision
				dynBody->velocity.y *= -1;

				// Loss of energy
				dynBody->velocity.y *= MathUtils::Abs(1 - globalRestitution.y);
				dynBody->velocity.x *= MathUtils::Abs(1 - globalFriction.x);
			}
			else if (normal.y == 0) // Horizontal collision with vertical surface
			{
				// Left -> Right
				if (directionVec.x > 0)
				{
					dynBody->rect.x = body->GetPosition(InUnit::IN_METERS).x - dynBody->rect.w;
					dynBody->bodyState.Set((int)BodyState::RIGHT, true);
				}
				// Right -> Left
				if (directionVec.x < 0)
				{
					dynBody->rect.x = body->GetRect(InUnit::IN_METERS).GetPosition(Align::CENTER_RIGHT).x;
					dynBody->bodyState.Set((int)BodyState::LEFT, true);
				}

				// Perfectly elastic collision
				dynBody->velocity.x *= -1;

				// Loss of energy
				dynBody->velocity.x *= MathUtils::Abs(1 - globalRestitution.x);
				dynBody->velocity.y *= MathUtils::Abs(1 - globalFriction.y);
			}
			else
			{
				Rect wtfRect = c->GetBody()->GetRect(InUnit::IN_METERS);
				Point wtfOrgn = dynBody->backup.position;
				Point wtfDir = directionVec;
				LOG("Weird Rectangle: X = %.f, Y = %.f, Width = %.f, Heigth = %.f\nWeird Ray: X = %.f, Y = %.f, Dir X = %.f, Dir Y = %.f", wtfRect.x, wtfRect.y, wtfRect.w, wtfRect.h, wtfOrgn.x, wtfOrgn.y, wtfDir.x, wtfDir.y);
				assert("This is impossible, how could this happen???? :O Quick, check the log!!!!");
			}

			break;
		}

		case BodyClass::LIQUID_BODY:
		{
			LiquidBody* body = (LiquidBody*)b;
			float submergedVolume = intersect.GetArea();
			// INFO: this formula is: Density * Volume submerged * gravity * buoyancy. I give just 1/volume because ApplyForce internally applies mass, so 1/volume * mass = density
			//-TODO: Check whether we need to multiply the mass inside AddForce function or create an "AddForceInternal" that avoids multiplying by the mass
			float forcex = body->GetDensity(InUnit::IN_METERS) * submergedVolume * (globalGravity.x + dynBody->GetGravityOffset(InUnit::IN_METERS).x) * body->GetBuoyancy();
			float forcey = body->GetDensity(InUnit::IN_METERS) * submergedVolume * (globalGravity.y + dynBody->GetGravityOffset(InUnit::IN_METERS).y) * body->GetBuoyancy();
			dynBody->forces.emplace_back(new Force(Point{ -forcex, -forcey }, InUnit::IN_METERS));
		}

		default:
			break;
		}

		// If collision debugging is disabled
		if (!physicsConfig->Get(0)) RELEASE(c);
	}

	// If collision debugging is disabled
	if (!physicsConfig->Get(0)) collisions.clear();
}