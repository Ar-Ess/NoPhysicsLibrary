#include "Physics.h"
#include "LiquidBody.h"
#include "GasBody.h"
#include "DynamicBody.h"
#include "StaticBody.h"

#include "PhysMath.h"

Physics::Physics(const PhysArray<Body*>* bodies, const Flag* physicsConfig, const PhysArray<unsigned int*>* gasIndex, const PhysArray<unsigned int*>* liquidIndex, const float* pixelsToMeters, const unsigned int* physIterations)
{
	this->bodies = bodies;
	this->physicsConfig = physicsConfig;
	this->gasIndex = gasIndex;
	this->liquidIndex = liquidIndex;
	this->pixelsToMeters = pixelsToMeters;
	this->physIterations = physIterations;
}

Physics::~Physics()
{}

void Physics::Step(Body* b, float dt)
{
	if (b->Class() != BodyClass::DYNAMIC_BODY) return;
	DynamicBody* body = (DynamicBody*)b;

	if (!body->properties.Get(1)) return;

	// Apply gas and liquid forces
	ApplyNaturalForces(body);

	// Multiplying global gravity * mass to acquire the force (Global gravity falls :) )
	body->ApplyForce(globalGravity * b->Mass(), InUnit::IN_METERS);

	// Multiplying intrinsic gravity * mass to acquire the force (Local gravity falls :) )
	body->ApplyForce(body->gravityOffset * b->Mass(), InUnit::IN_METERS);

	// Second law newton
	body->SecondNewton(); // Suma de forces a acceleració

	// First law Buxeda
	body->FirstBuxeda(); // Suma de momentum a velocity

	// Generate & save previous instance of body
	body->Backup();

	// Integrate
	Integrate(dt, b);
}

void Physics::SolveCollisions(PhysArray<Body*>* bodies)
{
	ResetFlags(bodies);

	for (int i = 0; i < *physIterations; ++i)
	{
		DetectCollisions(bodies);

		Declip();
	}
}

void Physics::CleanUp()
{
	collisions.Clear();
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

void Physics::ApplyNaturalForces(Body* dynBody)
{
	if (!liquidIndex->Empty()) ApplyHydroForces(dynBody);

	if (!gasIndex->Empty()) ApplyAeroForces(dynBody);
}

void Physics::ApplyHydroForces(Body* dynBody)
{
	DynamicBody* body = (DynamicBody*)dynBody;

	if (!body->IsBodyStill(BodyState::IN_LIQUID)) return;

	const float fullArea = body->Rect(InUnit::IN_METERS).Area();
	float areaCovered = 0;
	for (unsigned int index = 0; index < liquidIndex->Size(); ++index)
	{
		unsigned int i = *liquidIndex->At(index);
		Body* liquid = bodies->At(i);

		if (body->IsIdExcludedFromCollision(liquid->Id())) continue;

		if (!PhysMath::CheckCollision(liquid->Rect(InUnit::IN_METERS), body->Rect(InUnit::IN_METERS))) continue;
		float area = PhysMath::IntersectRectangle(liquid->Rect(InUnit::IN_METERS), body->Rect(InUnit::IN_METERS)).Area();
		areaCovered += area;

		ApplyHydroDrag(body, liquid);
		//-TODO: End HydroForces
		ApplyHydroLift(body, liquid);
		ApplyBuoyancy(body, liquid, area);

		if (areaCovered - fullArea < 0.0001f) break;
	}
}

void Physics::ApplyHydroDrag(Body* dynBody, Body* env)
{
	LiquidBody* liquid = (LiquidBody*)env;
	DynamicBody* body = (DynamicBody*)dynBody;
	int negx = -1;
	int negy = -1;
	if (body->velocity.x < 0) negx *= -1;
	if (body->velocity.y < 0) negy *= -1;
	float hydrodragY = negy * 0.5 * liquid->Density(InUnit::IN_METERS) * PhysMath::Pow(body->velocity.y, 2) * body->Rect(InUnit::IN_METERS).h /* * liquid->GetDragCoefficient().y*/;
	float hydrodragX = negx * 0.5 * liquid->Density(InUnit::IN_METERS) * PhysMath::Pow(body->velocity.x, 2) * body->Rect(InUnit::IN_METERS).w /* * liquid->GetDragCoefficient().x*/;
	// negx *= -1;
	// negy *= -1;
	// float hydrodragY = negy * MathUtils::Abs(1 - liquid->GetDragCoefficient().y) * body->velocity.y;
	//float hydrodragX = negx * MathUtils::Abs(1 - liquid->GetDragCoefficient().x) * body->velocity.x;

	body->ApplyForce(hydrodragX, hydrodragY, InUnit::IN_METERS);
	//-TODO: Change the formula to this one -> neg * b * V. B is drag coefficient. I can split velocity iei
	// DONE, BUT IT SEEMS WEIRD...
}

void Physics::ApplyHydroLift(Body* dynBody, Body* env)
{
	LiquidBody* liquid = (LiquidBody*)env;
	DynamicBody* body = (DynamicBody*)dynBody;
	int negx = -1;
	int negy = 1;
	if (body->velocity.x < 0) negx = 1;
	if (body->velocity.y < 0) negy = -1;
	float hydroliftY = negx * 0.5 * liquid->Density(InUnit::IN_METERS) * PhysMath::Pow(body->velocity.x, 2) * body->Rect(InUnit::IN_METERS).h * liquid->LiftCoefficient().x;
	float hydroliftX = negy * 0.5 * liquid->Density(InUnit::IN_METERS) * PhysMath::Pow(body->velocity.y, 2) * body->Rect(InUnit::IN_METERS).w * liquid->LiftCoefficient().y;
	// La atrocitat probocada aquí constitueix la rotació de 90º en forma de x = -y i y = x (El lift sempre va a 90 graus respecte el drag)
	if (hydroliftY > 0) hydroliftY *= -1; // Maintain y axis lift always looking up :D
	body->ApplyForce(hydroliftX, hydroliftY, InUnit::IN_METERS);
}

void Physics::ApplyBuoyancy(Body* dynBody, Body* env, float area)
{
	LiquidBody* liquid = (LiquidBody*)env;
	DynamicBody* body = (DynamicBody*)dynBody;
	// Density * Area * gravity force (+ buoyancy coeficcient)
	float forcex = liquid->Density(InUnit::IN_METERS) * area * (globalGravity.x + body->GravityOffset(InUnit::IN_METERS).x) * liquid->Buoyancy();
	float forcey = liquid->Density(InUnit::IN_METERS) * area * (globalGravity.y + body->GravityOffset(InUnit::IN_METERS).y) * liquid->Buoyancy();
	body->ApplyForce(-forcex, -forcey , InUnit::IN_METERS);
}

void Physics::ApplyAeroForces(Body* dynBody)
{
	DynamicBody* body = (DynamicBody*)dynBody;
	if (!body->IsBodyStill(BodyState::IN_GAS)) return;

	const float fullArea = body->Rect(InUnit::IN_METERS).Area();
	float areaCovered = 0;
	for (unsigned int index = 0; index < gasIndex->Size(); ++index)
	{
		unsigned int i = *gasIndex->At(index);
		Body* gas = bodies->At(i);

		if (body->IsIdExcludedFromCollision(gas->Id())) continue;

		if (!PhysMath::CheckCollision(gas->Rect(InUnit::IN_METERS), body->Rect(InUnit::IN_METERS))) continue;
		float area = PhysMath::IntersectRectangle(gas->Rect(InUnit::IN_METERS), body->Rect(InUnit::IN_METERS)).Area();
		areaCovered += area;

		ApplyAeroDrag(body, gas, area);
		ApplyAeroLift(body, gas, area);

		if (areaCovered - fullArea < 0.0001f) break;
	}
}

void Physics::ApplyAeroDrag(Body* dynBody, Body* env, float area)
{
	DynamicBody* body = (DynamicBody*)dynBody;
	GasBody* gas = (GasBody*)env;
	int negx = -1;
	int negy = -1;
	if (body->velocity.x < 0) negx = 1;
	if (body->velocity.y < 0) negy = 1;
	float aerodragX = negx * 0.5 * gas->GetDensity(InUnit::IN_METERS) * PhysMath::Pow(body->velocity.x, 2) * area * gas->DragCoefficient().x;
	float aerodragY = negy * 0.5 * gas->GetDensity(InUnit::IN_METERS) * PhysMath::Pow(body->velocity.y, 2) * area * gas->DragCoefficient().y;
	body->ApplyForce(aerodragX, aerodragY, InUnit::IN_METERS);
}

void Physics::ApplyAeroLift(Body* dynBody, Body* env, float area)
{
	DynamicBody* body = (DynamicBody*)dynBody;
	GasBody* gas = (GasBody*)env;

	int negx = -1;
	int negy = 1;
	if (body->velocity.x < 0) negx = 1;
	if (body->velocity.y < 0) negy = -1;
	float aeroliftY = negx * 0.5 * gas->Density(InUnit::IN_METERS) * PhysMath::Pow(body->velocity.x, 2) * body->Rect(InUnit::IN_METERS).h * gas->LiftCoefficient().x;
	float aeroliftX = negy * 0.5 * gas->Density(InUnit::IN_METERS) * PhysMath::Pow(body->velocity.y, 2) * body->Rect(InUnit::IN_METERS).w * gas->LiftCoefficient().y;
	// La atrocitat probocada aquí constitueix la rotació de 90º en forma de x = -y i y = x (El lift sempre va a 90 graus respecte el drag)
	if (aeroliftY > 0) aeroliftY *= -1; // Maintain y axis lift always looking up :D
	body->ApplyForce(aeroliftX, aeroliftY, InUnit::IN_METERS);
}

void Physics::DetectCollisions(PhysArray<Body*>* bodies)
{
	// If collisions debugging is enabled
	if (physicsConfig->Get(0)) collisions.Clear();

	// New (iterate dynamic bodies with all)
	for (unsigned int i = 0; i < bodies->Size(); ++i)
	{
		Body* b = bodies->At(i);
		if (b->Class() != BodyClass::DYNAMIC_BODY) continue;
		if (!b->IsCollidable()) continue;

		DynamicBody* b1 = (DynamicBody*)b;

		if (!b1->properties.Get(1)) continue;

		for (unsigned int a = 0; a < bodies->Size(); ++a)
		{
			if (a == i) continue;
			Body* b2 = bodies->At(a);
			if (b2->Class() == BodyClass::GAS_BODY || b2->Class() == BodyClass::LIQUID_BODY || !b2->IsCollidable()) continue;
			if (b2->Class() == BodyClass::DYNAMIC_BODY && a < i) continue;
			if (b1->IsIdExcludedFromCollision(b2->Id())) continue;

			PhysRect intersect = PhysMath::IntersectRectangle(b1->Rect(InUnit::IN_METERS), b2->Rect(InUnit::IN_METERS));
			PhysVec intersectionPoint = {};
			// Try to check a collision
			if (!PhysMath::CheckCollision(b1->Rect(InUnit::IN_METERS), b2->Rect(InUnit::IN_METERS)))
			{
				continue;
				// In Process
				// Try to check if tunneling
				if (!PhysMath::RayCast(
					PhysRay(b1->Position(InUnit::IN_METERS) + (b1->Size(InUnit::IN_METERS) / 2), b1->backup.rectangle.Position() + (b1->backup.rectangle.Size() / 2)),
					b2->Rect(InUnit::IN_METERS),
					intersectionPoint
				)) continue;
			}

			collisions.Add(new Collision(b1, b2, intersect, pixelsToMeters));
		}
	}
}

void Physics::ResetFlags(PhysArray<Body*>* bodies)
{
	for (unsigned int i = 0; i < bodies->Size(); ++i)
	{
		Body* b = bodies->At(i);
		if (b->Class() != BodyClass::DYNAMIC_BODY) continue;
		DynamicBody* body = (DynamicBody*)b;
		body->bodyStateEnter.Clear();
		body->bodyStateStay.Clear();
		body->bodyStateExit.Clear();
	}
}

void Physics::Declip()
{
	for (unsigned int i = 0; i < collisions.Size(); ++i)
	{
		Collision* c = collisions.At(i);
		DynamicBody* dynBody = (DynamicBody*)c->DynBody();
		const Body* b = c->OtherBody();

		PhysRect intersect = c->GetCollisionRectangle(InUnit::IN_METERS);
		PhysVec directionVec = dynBody->Position(InUnit::IN_METERS) - dynBody->backup.rectangle.Position();

		switch (b->Class())
		{

		case BodyClass::DYNAMIC_BODY:
		{
			DynamicBody* body = (DynamicBody*)b;
			PhysVec directionVecAlter = body->Position(InUnit::IN_METERS) - body->backup.rectangle.Position();
			PhysVec normal = {};

			PhysRect cRect = c->GetCollisionRectangle(InUnit::IN_METERS);
			PhysVec centerOfIntersecion = cRect.Position() + (cRect.Size() / 2);
			PhysRay ray(centerOfIntersecion + (directionVec * -1.0f), centerOfIntersecion);
			if (!PhysMath::RayCast(ray, body->Rect(InUnit::IN_METERS), normal)) break;

			if (normal.x == 0) // Vertical collision with horizontal surface
			{
				// Top -> Bottom
				if (directionVec.y > 0)
				{
					dynBody->rect.y -= intersect.h / 2;
					body->rect.y += intersect.h / 2;
					if (!dynBody->prevBodyState.Get((int)BodyState::ON_GROUND)) dynBody->bodyStateEnter.Set((int)BodyState::ON_GROUND, true);
					dynBody->bodyStateStay.Set((int)BodyState::ON_GROUND, true);
				}
				// Bottom -> Top
				if (directionVec.y < 0)
				{
					dynBody->rect.y += intersect.h / 2;
					body->rect.y -= intersect.h / 2;
					if (!dynBody->prevBodyState.Get((int)BodyState::ON_ROOF)) dynBody->bodyStateEnter.Set((int)BodyState::ON_ROOF, true);
					dynBody->bodyStateStay.Set((int)BodyState::ON_ROOF, true);
				}

				// Perfectly elastic collision
				dynBody->velocity.y *= -1;

				// Loss of energy
				dynBody->velocity.y *= globalRestitution.y;
				dynBody->velocity.x *= PhysMath::Abs(1 - globalFriction.x);

			}
			else if (normal.y == 0) // Horizontal collision with vertical surface
			{
				// Left -> Right
				if (directionVec.x > 0)
				{
					dynBody->rect.x -= intersect.w / 2;
					body->rect.x += intersect.w / 2;
					if (!dynBody->prevBodyState.Get((int)BodyState::ON_RIGHT)) dynBody->bodyStateEnter.Set((int)BodyState::ON_RIGHT, true);
					dynBody->bodyStateStay.Set((int)BodyState::ON_RIGHT, true);
				}
				// Right -> Left
				if (directionVec.x < 0)
				{
					dynBody->rect.x += intersect.w / 2;
					body->rect.x -= intersect.w / 2;
					if (!dynBody->prevBodyState.Get((int)BodyState::ON_LEFT)) dynBody->bodyStateEnter.Set((int)BodyState::ON_LEFT, true);
					dynBody->bodyStateStay.Set((int)BodyState::ON_LEFT, true);
				}

				// Perfectly elastic collision
				dynBody->velocity.x *= -1;

				// Loss of energy
				dynBody->velocity.x *= globalRestitution.x;
				dynBody->velocity.y *= PhysMath::Abs(1 - globalFriction.y);

			}
			else
			{
				PhysRect wtfRect = c->OtherBody()->Rect(InUnit::IN_METERS);
				PhysVec wtfOrgn = dynBody->backup.rectangle.Position();
				PhysVec wtfDir = directionVec;
				LOG("Weird Rectangle: X = %.f, Y = %.f, Width = %.f, Heigth = %.f\nWeird Ray: X = %.f, Y = %.f, Dir X = %.f, Dir Y = %.f", wtfRect.x, wtfRect.y, wtfRect.w, wtfRect.h, wtfOrgn.x, wtfOrgn.y, wtfDir.x, wtfDir.y);
				assert("This is impossible, how could this happen???? :O Quick, check the log!!!!");
			}

			break;
		}

		case BodyClass::STATIC_BODY:
		{
			StaticBody* body = (StaticBody*)b;
			PhysVec normal = {};

			PhysRect cRect = c->GetCollisionRectangle(InUnit::IN_METERS);
			PhysVec centerOfIntersecion = cRect.Position() + (cRect.Size() / 2);
			PhysRay ray(centerOfIntersecion + (directionVec * -1.0f), centerOfIntersecion);
			if (!PhysMath::RayCast(ray, body->Rect(InUnit::IN_METERS), normal)) break;

			if (normal.x == 0) // Vertical collision with horizontal surface
			{
				// Top -> Bottom
				if (directionVec.y > 0)
				{
					dynBody->rect.y = body->Position(InUnit::IN_METERS).y - dynBody->rect.h;
					if (!dynBody->prevBodyState.Get((int)BodyState::ON_GROUND)) dynBody->bodyStateEnter.Set((int)BodyState::ON_GROUND, true);
					dynBody->bodyStateStay.Set((int)BodyState::ON_GROUND, true);
				}
				// Bottom -> Top
				if (directionVec.y < 0)
				{
					dynBody->rect.y = body->Rect(InUnit::IN_METERS).y + body->Rect(InUnit::IN_METERS).h;
					if (!dynBody->prevBodyState.Get((int)BodyState::ON_ROOF)) dynBody->bodyStateEnter.Set((int)BodyState::ON_ROOF, true);
					dynBody->bodyStateStay.Set((int)BodyState::ON_ROOF, true);
				}

				// Perfectly elastic collision
				dynBody->velocity.y *= -1;

				// Loss of energy
				dynBody->velocity.y *= (globalRestitution.y + body->RestitutionOffset().y + dynBody->RestitutionOffset().y);
				dynBody->velocity.x *= CalculateFriction(dynBody).x;
			}
			else if (normal.y == 0) // Horizontal collision with vertical surface
			{
				// Left -> Right
				if (directionVec.x > 0)
				{
					dynBody->rect.x = body->Position(InUnit::IN_METERS).x - dynBody->rect.w;
					if (!dynBody->prevBodyState.Get((int)BodyState::ON_RIGHT)) dynBody->bodyStateEnter.Set((int)BodyState::ON_RIGHT, true);
					dynBody->bodyStateStay.Set((int)BodyState::ON_RIGHT, true);
				}
				// Right -> Left
				if (directionVec.x < 0)
				{
					dynBody->rect.x = body->Rect(InUnit::IN_METERS).x + body->Rect(InUnit::IN_METERS).w;
					if (!dynBody->prevBodyState.Get((int)BodyState::ON_LEFT)) dynBody->bodyStateEnter.Set((int)BodyState::ON_LEFT, true);
					dynBody->bodyStateStay.Set((int)BodyState::ON_LEFT, true);
				}

				// Perfectly elastic collision
				dynBody->velocity.x *= -1;

				// Loss of energy
				dynBody->velocity.x *= (globalRestitution.x + body->RestitutionOffset().x + dynBody->RestitutionOffset().x);
				dynBody->velocity.y *= CalculateFriction(dynBody).y;
			}
			else
			{
				PhysRect wtfRect = c->OtherBody()->Rect(InUnit::IN_METERS);
				PhysVec wtfOrgn = dynBody->backup.rectangle.Position();
				PhysVec wtfDir = directionVec;
				LOG("Weird Rectangle: X = %.f, Y = %.f, Width = %.f, Heigth = %.f\nWeird Ray: X = %.f, Y = %.f, Dir X = %.f, Dir Y = %.f", wtfRect.x, wtfRect.y, wtfRect.w, wtfRect.h, wtfOrgn.x, wtfOrgn.y, wtfDir.x, wtfDir.y);
				assert("This is impossible, how could this happen???? :O Quick, check the log!!!!");
			}

			break;
		}

		default:
			break;
		}
	}

	// If collision debugging is disabled
	if (!physicsConfig->Get(0)) collisions.Clear();
}

PhysVec Physics::CalculateFriction(Body* dynBody)
{
	DynamicBody* body = (DynamicBody*)dynBody;
	PhysVec gF = { PhysMath::Abs(1 - globalFriction.x), PhysMath::Abs(1 - globalFriction.y) };
	PhysVec outFriction = {gF.x - body->frictionOffset.x, gF.y - body->frictionOffset.y};

	PhysMath::Clamp(outFriction.x, 0, 1);
	PhysMath::Clamp(outFriction.y, 0, 1);

	return outFriction;
}
