#include "Physics.h"
#include "LiquidBody.h"
#include "DynamicBody.h"
#include "StaticBody.h"

#include "MathUtils.h"
#include "Ray.h"

Physics::Physics(const Flag* physicsConfig)
{
	this->physicsConfig = physicsConfig;
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

void Physics::SolveCollisions(std::vector<Body*>* bodies)
{
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
	// If collisions debugging is enabled
	if (physicsConfig->Get(0))
	{
		if (!collisions.empty())
		{
			for (Collision* c : collisions) RELEASE(c);

				collisions.clear();
		}
	}

	size_t size = bodies->size();
	for (unsigned int i = 0; i < size - 1; ++i)
	{
		Body* b1 = bodies->at(i);
		if (b1->GetClass() == BodyClass::GAS_BODY || b1->GetClass() == BodyClass::LIQUID_BODY) continue;

		for (unsigned int a = 1 + i; a < size; ++a)
		{
			Body* b2 = bodies->at(a);
			if (b2->GetClass() == BodyClass::GAS_BODY || b2->GetClass() == BodyClass::LIQUID_BODY) continue;
			if (b1->GetClass() == BodyClass::STATIC_BODY && b2->GetClass() == BodyClass::STATIC_BODY) continue;

			Rect intersect = MathUtils::IntersectRectangle(b1->GetRect(), b2->GetRect());
			if (!MathUtils::CheckCollision(b1->GetRect(), b2->GetRect())) continue;

			Body* dyn = b2;
			Body* other = b2;
			b1->GetClass() == BodyClass::DYNAMIC_BODY ? dyn = b1 : other = b1;

			collisions.emplace_back(new Collision(dyn, other, intersect));
		}
	}
}

void Physics::Declip()
{
	for (Collision* c : collisions)
	{
		DynamicBody* dynBody = (DynamicBody*)c->GetDynBody();
		Rect intersect = c->GetCollisionRectangle();

		Point directionVec = dynBody->GetPosition() - dynBody->backup.position;

		switch (c->GetBody()->GetClass())
		{

		case BodyClass::DYNAMIC_BODY:
		{
			//-Todone: raycast from center and detect intersection from infinite (just w/2 or h/2 of dynamic) plans of the static body
			// prioritize go back in time rather than linear declipping :_)
			DynamicBody* body = (DynamicBody*)c->GetBody();
			Point directionVecAlter = body->GetPosition() - body->backup.position;
			Point normal = {};

			Point centerOfIntersecion = c->GetCollisionRectangle().GetPosition(Align::CENTER);
			Ray ray(centerOfIntersecion.Apply(directionVec.Multiply(-1)), centerOfIntersecion);
			if (!MathUtils::RayCast(ray, body->GetRect(), normal)) break;

			if (normal.x == 0) // Vertical
			{
				// Top -> Bottom
				if (directionVec.y > 0)
				{
					dynBody->rect.y -= intersect.h / 2;
					body->rect.y += intersect.h / 2;
				}
				// Bottom -> Top
				if (directionVec.y < 0)
				{
					dynBody->rect.y += intersect.h / 2;
					body->rect.y -= intersect.h / 2;
				}

				dynBody->velocity.y = 0;
			}
			else if (normal.y == 0) // Horizontal
			{
				// Left -> Right
				if (directionVec.x > 0)
				{
					dynBody->rect.x -= intersect.w / 2;
					body->rect.x += intersect.w / 2;
				}
				// Right -> Left
				if (directionVec.x < 0)
				{
					dynBody->rect.x += intersect.w / 2;
					body->rect.x -= intersect.w / 2;
				}

				dynBody->velocity.x = 0;
			}
			else
			{
				Rect wtfRect = c->GetBody()->GetRect();
				Point wtfOrgn = dynBody->backup.position;
				Point wtfDir = directionVec;
				LOG("Weird Rectangle: X = %.f, Y = %.f, Width = %.f, Heigth = %.f\nWeird Ray: X = %.f, Y = %.f, Dir X = %.f, Dir Y = %.f", wtfRect.x, wtfRect.y, wtfRect.w, wtfRect.h, wtfOrgn.x, wtfOrgn.y, wtfDir.x, wtfDir.y);
				assert("This is impossible, how could this happen???? :O Quick, check the log!!!!");
			}

			break;
		}

		case BodyClass::STATIC_BODY:
		{
			//-Todone: raycast from center and detect intersection from infinite (just w/2 or h/2 of dynamic) plans of the static body
			// prioritize go back in time rather than linear declipping :_)
			StaticBody* body = (StaticBody*)c->GetBody();
			Point normal = {};

			Point centerOfIntersecion = c->GetCollisionRectangle().GetPosition(Align::CENTER);
			Ray ray(centerOfIntersecion.Apply(directionVec.Multiply(-1)), centerOfIntersecion);
			if (!MathUtils::RayCast(ray, body->GetRect(), normal)) break;

			if (normal.x == 0) // Vertical
			{
				// Top -> Bottom
				if (directionVec.y > 0) dynBody->rect.y = body->GetPosition().y - dynBody->rect.h;
				// Bottom -> Top
				if (directionVec.y < 0) dynBody->rect.y = body->GetRect().GetPosition(Align::BOTTOM_CENTER).y;

				dynBody->velocity.y = 0;
			}
			else if (normal.y == 0) // Horizontal
			{
				// Left -> Right
				if (directionVec.x > 0) dynBody->rect.x = body->GetPosition().x - dynBody->rect.w;
				// Right -> Left
				if (directionVec.x < 0) dynBody->rect.x = body->GetRect().GetPosition(Align::CENTER_RIGHT).x;

				dynBody->velocity.x = 0;
			}
			else
			{
				Rect wtfRect = c->GetBody()->GetRect();
				Point wtfOrgn = dynBody->backup.position;
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