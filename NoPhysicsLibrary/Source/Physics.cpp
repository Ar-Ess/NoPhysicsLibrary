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

void Physics::Step(Body* b, float dt)
{
	if (b->GetClass() != BodyClass::DYNAMIC_BODY) return;

	DynamicBody* body = (DynamicBody*)b;

	// Apply gas and liquid forces
	ApplyNaturalForces(body);

	// Multiplying global gravity * mass to acquire the force (Global gravity falls :) )
	body->ApplyForce(globalGravity, InUnit::IN_METERS);

	// Multiplying intrinsic gravity * mass to acquire the force (Local gravity falls :) )
	body->ApplyForce(body->gravityOffset, InUnit::IN_METERS);

	// Second law newton
	body->SecondNewton(); // Suma de forces a acceleració

	// First law Buxeda
	body->FirstBuxeda(); // Suma de momentum a velocity

	// Generate & save previous instance of body
	body->Backup();

	// Integrate
	Integrate(dt, b);
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

void Physics::Integrate(float dt, Body* b)
{
	DynamicBody* body = (DynamicBody*)b;

	// Second Order Red Velvet Integrator
	body->rect.x += body->velocity.x * dt + 0.5f * body->acceleration.x * dt * dt;
	body->velocity.x += body->acceleration.x * dt;

	body->rect.y += body->velocity.y * dt + 0.5f * body->acceleration.y * dt * dt;
	body->velocity.y += body->acceleration.y * dt;
}

void Physics::ApplyNaturalForces(DynamicBody* body)
{
	if (!liquidIndex->empty()) ApplyHydroForces(body);

	if (!gasIndex->empty()) ApplyAeroForces(body);
}

void Physics::ApplyHydroForces(DynamicBody* body)
{
	if (!body->IsBody(BodyState::IN_LIQUID)) return;

	const float fullArea = body->GetRect(InUnit::IN_METERS).GetArea();
	float areaCovered = 0;
	for (unsigned int* i : *liquidIndex)
	{
		Body* liquid = bodies->at(*i);

		if (!MathUtils::CheckCollision(liquid->GetRect(InUnit::IN_METERS), body->GetRect(InUnit::IN_METERS))) continue;
		float area = MathUtils::IntersectRectangle(bodies->at(*i)->GetRect(InUnit::IN_METERS), body->GetRect(InUnit::IN_METERS)).GetArea();
		areaCovered += area;

		ApplyHydroDrag(body, liquid);
		//-TODO: End HydroForces
		ApplyHydroLift(body, liquid, area);
		ApplyBuoyancy(body, liquid, area);

		if (areaCovered - fullArea < 0.0001f) break;
	}
}

void Physics::ApplyHydroDrag(DynamicBody* body, Body* env)
{
	//-TODO: Check whether we need to multiply the mass inside AddForce function or create an "AddForceInternal" that avoids multiplying by the mass
	//-TODO: check if this is actually working
	LiquidBody* liquid = (LiquidBody*)env;
	int negx = -1;
	int negy = -1;
	if (body->velocity.x < 0) negx *= -1;
	if (body->velocity.y < 0) negy *= -1;                                                                      //-TOCHECK: check if this is right. The function acts with the projected area. I input the "submerjed area or the width?"
	float hydrodragX = negx * 0.5 * liquid->GetDensity(InUnit::IN_METERS) * MathUtils::Pow(body->velocity.x, 2) * body->GetRect(InUnit::IN_METERS).w /* * liquid->GetDragCoefficient().x*/;
	float hydrodragY = negy * 0.5 * liquid->GetDensity(InUnit::IN_METERS) * MathUtils::Pow(body->velocity.y, 2) * body->GetRect(InUnit::IN_METERS).h /* * liquid->GetDragCoefficient().y*/;
	body->forces.emplace_back(new Force(hydrodragX, hydrodragY, InUnit::IN_METERS));
}

void Physics::ApplyHydroLift(DynamicBody* body, Body* env, float area)
{
	LiquidBody* liquid = (LiquidBody*)env;
}

void Physics::ApplyBuoyancy(DynamicBody* body, Body* env, float area)
{
	LiquidBody* liquid = (LiquidBody*)env;
	float forcex = liquid->GetDensity(InUnit::IN_METERS) * area * (globalGravity.x + body->GetGravityOffset(InUnit::IN_METERS).x) * liquid->GetBuoyancy();
	float forcey = liquid->GetDensity(InUnit::IN_METERS) * area * (globalGravity.y + body->GetGravityOffset(InUnit::IN_METERS).y) * liquid->GetBuoyancy();
	body->forces.emplace_back(new Force(-forcex, -forcey , InUnit::IN_METERS));
}

void Physics::ApplyAeroForces(DynamicBody* body)
{
	if (!body->IsBody(BodyState::IN_GAS)) return;

	const float fullArea = body->GetRect(InUnit::IN_METERS).GetArea();
	float areaCovered = 0;
	for (unsigned int* i : *gasIndex)
	{
		Body* gas = bodies->at(*i);

		if (!MathUtils::CheckCollision(gas->GetRect(InUnit::IN_METERS), body->GetRect(InUnit::IN_METERS))) continue;
		float area = MathUtils::IntersectRectangle(bodies->at(*i)->GetRect(InUnit::IN_METERS), body->GetRect(InUnit::IN_METERS)).GetArea();
		areaCovered += area;

		ApplyAeroDrag(body, gas, area);
		//-TODO: End AeroForces
		ApplyAeroLift(body, gas, area);

		if (areaCovered - fullArea < 0.0001f) break;
	}
}

void Physics::ApplyAeroDrag(DynamicBody* body, Body* env, float area)
{
	GasBody* gas = (GasBody*)env;
	//-TODO: Check whether we need to multiply the mass inside AddForce function or create an "AddForceInternal" that avoids multiplying by the mass
	int negx = -1;
	int negy = -1;
	if (body->velocity.x < 0) negx = 1;
	if (body->velocity.y < 0) negy = 1;
	float aerodragX = negx * 0.5 * gas->GetDensity() * MathUtils::Pow(body->velocity.x, 2) * area * gas->GetDragCoefficient().x;
	float aerodragY = negy * 0.5 * gas->GetDensity() * MathUtils::Pow(body->velocity.y, 2) * area * gas->GetDragCoefficient().y;
	body->forces.emplace_back(new Force(aerodragX, aerodragY, InUnit::IN_METERS));
}

void Physics::ApplyAeroLift(DynamicBody* body, Body* env, float area)
{
	GasBody* gas = (GasBody*)env;
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
		Body* b = bodies->at(i);
		if (b->GetClass() != BodyClass::DYNAMIC_BODY) continue;
		if (!b->IsCollidable()) continue;

		DynamicBody* b1 = (DynamicBody*)b;

		for (unsigned int a = 0; a < size; ++a)
		{
			if (a == i) continue;
			Body* b2 = bodies->at(a);
			if (b2->GetClass() == BodyClass::GAS_BODY || b2->GetClass() == BodyClass::LIQUID_BODY || !b2->IsCollidable()) continue;
			if (b2->GetClass() == BodyClass::DYNAMIC_BODY && a < i) continue;
			if (b1->IsIdExcludedFromCollision(b2->GetId())) continue;

			Rect intersect = MathUtils::IntersectRectangle(b1->GetRect(InUnit::IN_METERS), b2->GetRect(InUnit::IN_METERS));
			Point intersectionPoint = {};
			// Try to check a collision
			if (!MathUtils::CheckCollision(b1->GetRect(InUnit::IN_METERS), b2->GetRect(InUnit::IN_METERS)))
			{
				continue;
				// In Process
				// Try to check if tunneling
				if (!MathUtils::RayCast(
					Ray(b1->GetPosition(InUnit::IN_METERS, Align::CENTER), b1->backup.rectangle.GetPosition(Align::CENTER)),
					b2->GetRect(InUnit::IN_METERS),
					intersectionPoint
				)) continue;
			}

			collisions.emplace_back(new Collision(b1, b2, intersect, pixelsToMeters));
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
		Point directionVec = dynBody->GetPosition(InUnit::IN_METERS) - dynBody->backup.rectangle.GetPosition();

		switch (b->GetClass())
		{

		case BodyClass::DYNAMIC_BODY:
		{
			DynamicBody* body = (DynamicBody*)b;
			Point directionVecAlter = body->GetPosition(InUnit::IN_METERS) - body->backup.rectangle.GetPosition();
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
					dynBody->bodyState.Set((int)BodyState::ON_GROUND, true);
				}
				// Bottom -> Top
				if (directionVec.y < 0)
				{
					dynBody->rect.y += intersect.h / 2;
					body->rect.y -= intersect.h / 2;
					dynBody->bodyState.Set((int)BodyState::ON_ROOF, true);
				}

				// Perfectly elastic collision
				dynBody->velocity.y *= -1;

				// Loss of energy
				dynBody->velocity.y *= globalRestitution.y;
				dynBody->velocity.x *= MathUtils::Abs(1 - globalFriction.x);

			}
			else if (normal.y == 0) // Horizontal collision with vertical surface
			{
				// Left -> Right
				if (directionVec.x > 0)
				{
					dynBody->rect.x -= intersect.w / 2;
					body->rect.x += intersect.w / 2;
					dynBody->bodyState.Set((int)BodyState::ON_RIGHT, true);
				}
				// Right -> Left
				if (directionVec.x < 0)
				{
					dynBody->rect.x += intersect.w / 2;
					body->rect.x -= intersect.w / 2;
					dynBody->bodyState.Set((int)BodyState::ON_LEFT, true);
				}

				// Perfectly elastic collision
				dynBody->velocity.x *= -1;

				// Loss of energy
				dynBody->velocity.x *= globalRestitution.x;
				dynBody->velocity.y *= MathUtils::Abs(1 - globalFriction.y);

			}
			else
			{
				Rect wtfRect = c->GetBody()->GetRect(InUnit::IN_METERS);
				Point wtfOrgn = dynBody->backup.rectangle.GetPosition();
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
					dynBody->bodyState.Set((int)BodyState::ON_GROUND, true);
				}
				// Bottom -> Top
				if (directionVec.y < 0)
				{
					dynBody->rect.y = body->GetRect(InUnit::IN_METERS).GetPosition(Align::BOTTOM_CENTER).y;
					dynBody->bodyState.Set((int)BodyState::ON_ROOF, true);
				}

				// Perfectly elastic collision
				dynBody->velocity.y *= -1;

				// Loss of energy
				dynBody->velocity.y *= (globalRestitution.y + body->GetRestitutionOffset().y);
				dynBody->velocity.x *= MathUtils::Abs(1 - globalFriction.x);
			}
			else if (normal.y == 0) // Horizontal collision with vertical surface
			{
				// Left -> Right
				if (directionVec.x > 0)
				{
					dynBody->rect.x = body->GetPosition(InUnit::IN_METERS).x - dynBody->rect.w;
					dynBody->bodyState.Set((int)BodyState::ON_RIGHT, true);
				}
				// Right -> Left
				if (directionVec.x < 0)
				{
					dynBody->rect.x = body->GetRect(InUnit::IN_METERS).GetPosition(Align::CENTER_RIGHT).x;
					dynBody->bodyState.Set((int)BodyState::ON_LEFT, true);
				}

				// Perfectly elastic collision
				dynBody->velocity.x *= -1;

				// Loss of energy
				dynBody->velocity.x *= (globalRestitution.x + body->GetRestitutionOffset().x);
				dynBody->velocity.y *= MathUtils::Abs(1 - globalFriction.y);
			}
			else
			{
				Rect wtfRect = c->GetBody()->GetRect(InUnit::IN_METERS);
				Point wtfOrgn = dynBody->backup.rectangle.GetPosition();
				Point wtfDir = directionVec;
				LOG("Weird Rectangle: X = %.f, Y = %.f, Width = %.f, Heigth = %.f\nWeird Ray: X = %.f, Y = %.f, Dir X = %.f, Dir Y = %.f", wtfRect.x, wtfRect.y, wtfRect.w, wtfRect.h, wtfOrgn.x, wtfOrgn.y, wtfDir.x, wtfDir.y);
				assert("This is impossible, how could this happen???? :O Quick, check the log!!!!");
			}

			break;
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