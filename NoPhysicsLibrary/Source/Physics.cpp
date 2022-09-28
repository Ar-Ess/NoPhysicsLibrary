#include "Physics.h"
#include "LiquidBody.h"
#include "DynamicBody.h"

#include "MathUtils.h"

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
	// If collisions debugging is enabled
	if (physicsConfig->Get(0))
	{
		if (!collisions.empty())
		{
			for (Collision* c : collisions) RELEASE(c);

				collisions.clear();
		}
	}

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
			if (b1->GetClass() == BodyClass::STATIC_BODY && b2->GetClass() == BodyClass::STATIC_BODY) continue;

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
		DynamicBody* dynBody = (DynamicBody*)c->GetDynBody();
		Rect intersect = c->GetCollisionRectangle();

		Point dDeltaPosVec = dynBody->GetPosition() - dynBody->backup.position;
		switch (c->GetBody()->GetClass())
		{

		case BodyClass::DYNAMIC_BODY:
		{
			DynamicBody* body = (DynamicBody*)c->GetBody();
			Point bDeltaPosVec = body->GetPosition() - body->backup.position;

			break;
		}

		case BodyClass::STATIC_BODY:
		{
			// The dynamic rectangle is not half colliding with the static rectangle. This assures that is fully inside or overleaking from both sides (case 1)
			if (dynBody->rect.w >= intersect.w || dynBody->rect.h >= intersect.h)
			{
				//-TOCHECK: This method can break when the dynamic body is going really fast and overcomes completely the static body. (Case 3 & 4)
				// The solution could be tracing a line between the prev rect & the actual rect and see in which wall of the static rect intersects
				// As well, if the static body is really thin and the dynamic body fast, this method does not work. It would confuse it with the oposite decliping
				
				// Vertical collision (deprecated)
				if (intersect.w > intersect.h)
				{
					// Collision from the top
					if (intersect.GetPosition(Alignment::BOTTOM_LEFT).y == dynBody->rect.GetPosition(Alignment::BOTTOM_LEFT).y)
					{
						dynBody->rect.y -= intersect.h;
					}
					// Collision from the bottom
					else if (intersect.y == dynBody->rect.y)
					{
						dynBody->rect.y += intersect.h;
					}
					// Dynamic body overpass vertically form the other side the static body
					else
					{
						// Here is where the problem begins with this method
					}
				}
				// Horizontal collision (deprecated)
				else if (intersect.h > intersect.w)
				{
					// Collision from the left
					if (intersect.GetPosition(Alignment::TOP_RIGHT).x == dynBody->rect.GetPosition(Alignment::TOP_RIGHT).y)
					{
						dynBody->rect.x -= intersect.w;
					}
					// Collision from the right
					else if (intersect.x == dynBody->rect.x)
					{
						dynBody->rect.x += intersect.w;
					}
					// Dynamic body overpass horizontally form the other side the static body
					else
					{
						// Here is where the problem begins with this method
					}
				}
				// Square intersection, need to work with the deltaVector
				else
				{

				}
			}
			// The dynamic rectangle is half colliding with the static rectangle. Not all the body with or height is inside the static body, it is partially inside
			else
			{

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