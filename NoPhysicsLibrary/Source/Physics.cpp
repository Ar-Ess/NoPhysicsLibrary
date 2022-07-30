#include "Physics.h"
#include "GhostSlot.h"
#include "MathUtils.h"
#include "DynArray.h"

Physics::Physics()
{
}

Physics::~Physics()
{}

//void Physics::Draw(float dt, DynamicBody* dB)
//{
//	if (debug) DebugDraw();
//	if (debugBools) DebugBools(dB, dt);
//	ResetBodyBools();
//}
//
//bool Physics::CleanUp()
//{
//	std::vector<Body*>::const_iterator it;
//	for (it = bodies.begin(); bodies.empty(); it) DestroyBody(it);
//	
//	bodies.shrink_to_fit();
//	bodies.clear();
//
//	return true;
//}
//
//void Physics::DebugDraw()
//{
//	std::vector<Body*>::const_iterator it;
//	for (it = bodies.begin(); it != bodies.end(); ++it)
//	{
//		Body* body = (*it);
//		Rect rectx = {};
//		Rect rect = {};
//		Circle circx;
//		Circle circ;
//
//		if (body->isCollidable)
//		{
//			switch (body->colliderType)
//			{
//			case CollisionType::RECTANGLE:
//				rectx = { body->rect };
//				rect = { METERS_TO_PIXELS(rectx.x), METERS_TO_PIXELS(rectx.y), METERS_TO_PIXELS(rectx.w), METERS_TO_PIXELS(rectx.h) };
//
//				switch (body->bodyType)
//				{
//				case BodyType::DYNAMIC_BODY:
//					render->DrawRectangle(rect, { 255, 200, 0, 150 });
//					break;
//				case BodyType::STATIC_BODY:
//					render->DrawRectangle(rect, { 255, 0, 0, 150 });
//					break;
//				}
//				break;
//
//			case CollisionType::CIRCLE:
//				circx = body->circle;
//				circ = { (float)METERS_TO_PIXELS(circx.x), (float)METERS_TO_PIXELS(circx.y), (float)METERS_TO_PIXELS(circx.radius) };
//
//				switch (body->bodyType)
//				{
//				case BodyType::DYNAMIC_BODY:
//					render->DrawCircle(circ, { 255, 200, 0, 150 });
//					break;
//				case BodyType::STATIC_BODY:
//					render->DrawCircle(circ, { 255, 0, 0, 150 });
//					break;
//				}
//				break;
//			}
//		}
//	}
//}
//
//void Physics::DebugBools(DynamicBody* dB, float dt)
//{
//	bool dynamicProps = (dB != nullptr);
//	bool ground = false;
//	bool roof = false;
//	bool jump = false;
//	bool doubleJump = false;
//	bool lWall = false;
//	bool rWall = false;
//	bool wallJump = false;
//	bool dash = false;
//	bool air = false;
//	bool water = false;
//
//	if (dynamicProps) render->DrawRectangle({ 0, 0, 360, 135 }, {180, 180, 180, 200}, true, false);
//	else render->DrawRectangle({ 0, 0, 180, 135 }, { 180, 180, 180, 200 }, true, false);
//
//	if (dynamicProps)
//	{
//		char posStr[40] = {};
//		sprintf(posStr, "POS: %.2f %.2f", dB->position.x, dB->position.y);
//		//gui->ChangeString(10, posStr);
//
//		char velStr[40] = {};
//		sprintf(velStr, "VEL: %.2f %.2f", dB->velocity.x, dB->velocity.y);
//		//gui->ChangeString(11, velStr);
//
//		char accStr[40] = {};
//		float kgs = dB->mass / dt;
//		sprintf(accStr, "ACC: %.2f %.2f", (dB->velocity.x * kgs), (dB->velocity.y * kgs));
//		//gui->ChangeString(12, accStr);
//	}
//
//	int xOff = 52;
//	int xOff1 = xOff + 80;
//	int yOff = 2;
//	std::vector<Body*>::const_iterator it;
//	for (it = bodies.begin(); it != bodies.end(); ++it)
//	{
//		Body* body = (*it);
//		if (body->bodyType == BodyType::DYNAMIC_BODY)
//		{
//			DynamicBody* dB = (DynamicBody*)body;
//			if (dB->onGround && !ground)
//			{
//				render->DrawRectangle({ (float)xOff, 10.0f - (float)yOff, 25, 15 }, { 255, 0, 0, 255 }, true, false);
//				ground = true;
//			}
//			
//			if (dB->onLeftWall && !lWall)
//			{
//				render->DrawRectangle({ (float)xOff, 35 - (float)yOff, 25, 15 }, { 0, 255, 0, 255 }, true, false);
//				lWall = true;
//			}
//
//			if (dB->onRightWall && !rWall)
//			{
//				render->DrawRectangle({ (float)xOff, 60 - (float)yOff, 25, 15 }, { 255, 255, 0, 255 }, true, false);
//				rWall = true;
//			}
//			
//			if (dB->onRoof && !roof)
//			{
//				render->DrawRectangle({ (float)xOff, 85 - (float)yOff, 25, 15 }, { 0, 0, 255, 255 }, true, false);
//				roof = true;
//			}
//
//			if (dB->onAir && !air)
//			{
//				render->DrawRectangle({ (float)xOff, 110 - (float)yOff, 25, 15 }, { 225, 105, 55, 255 }, true, false);
//				air = true;
//			}
//
//			if (dB->onJump && !jump)
//			{
//				render->DrawRectangle({ (float)xOff1, 10 - (float)yOff, 25, 15 }, { 0, 255, 255, 255 }, true, false);
//				jump = true;
//			}
//
//			if (dB->onDoubleJump && !doubleJump)
//			{
//				render->DrawRectangle({ (float)xOff1, 35 - (float)yOff, 25, 15 }, { 255, 0, 255, 255 }, true, false);
//				doubleJump = true;
//			}
//
//			if (dB->onWallJump && !wallJump)
//			{
//				render->DrawRectangle({ (float)xOff1, 60 - (float)yOff, 25, 15 }, { 255, 255, 255, 255 }, true, false);
//				wallJump = true;
//			}
//
//			if (dB->onDash && !dash)
//			{
//				render->DrawRectangle({ (float)xOff1, 85 - (float)yOff, 25, 15 }, { 225, 55, 105, 255 }, true, false);
//				dash = true;
//			}
//
//			if (dB->onWater && !water)
//			{
//				render->DrawRectangle({ (float)xOff1, 110 - (float)yOff, 25, 15 }, { 55, 225, 105, 255 }, true, false);
//				water = true;
//			}
//		}
//	}
//}
//
//void Physics::ResetBodyBools()
//{
//	std::vector<Body*>::const_iterator it;
//	for (it = bodies.begin(); it != bodies.end(); ++it)
//	{
//		Body* body = (*it);
//		if (body->bodyType == BodyType::DYNAMIC_BODY)
//		{
//			DynamicBody* dB = (DynamicBody*)body;
//			dB->ResetBools();
//		}
//	}
//}
//

void Physics::Integrate(DynamicBody* body, float dt)
{
	// Second Order Red Velvet Integrator

	body->rect.x += body->velocity.x * dt + 0.5f * body->acceleration.x * dt * dt;
	body->velocity.x += body->acceleration.x * dt;

	body->rect.y += body->velocity.y * dt + 0.5f * body->acceleration.y * dt * dt;
	body->velocity.y += body->acceleration.y * dt;
}

void Physics::Update(float dt)
{
	for (Body* body : bodies)
	{
		switch (body->GetBodyClass())
		{
		case BodyClass::DYNAMIC_BODY: UpdateDynamic(dt, (DynamicBody*)body); break;
		case BodyClass::LIQUID_BODY: UpdateLiquid(dt); break;
		}
	}
}

void Physics::UpdateDynamic(float dt, DynamicBody* body)
{
	AutoApplyForces(); // Future

	// Multiplying gravity * mass to acquire the force
	body->ApplyForce(body->gravityAcceleration * body->mass);

	// Second law newton
	body->SecondNewton(); // Suma de forces a acceleració

	// First law Buxeda
	body->FirstBuxeda(); // Suma de momentum a velocity

	BodyBackUp backup = body->BackUp();

	// Integrate
	Integrate(body, dt);

	// Check Collisions
	CheckCollisions(body, backup);

}

void Physics::UpdateLiquid(float dt)
{
}

void Physics::AutoApplyForces()
{
	// TODO: Iteration check collision with environment

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

void Physics::DestroyBody(std::vector<Body*>::const_iterator it)
{
	Body* body = *it;
	delete body;
	bodies.erase(it);
	body = nullptr;
	bodies.shrink_to_fit();
}

//void Physics::SetGlobalGravity(Point gravity)
//{
//	if (gravity == Point{ 0.0f, 0.0f }) globalGravityActive = false;
//	else
//	{
//		globalGravityActive = true;
//		globalGravity = gravity;
//	}
//}
//
//void Physics::ChangeGravityAcceleration(Point acceleration)
//{
//	std::vector<Body*>::const_iterator it;
//	for (it = bodies.begin(); it != bodies.end(); ++it)
//	{
//		Body* body = (*it);
//		Point acc = acceleration;
//		switch (body->bodyType)
//		{
//		case BodyType::DYNAMIC_BODY:
//			DynamicBody* dB = (DynamicBody*)body;
//			acc *= REALITY_MULTIPLIER;
//			dB->gravityAcceleration = acc;
//			break;
//		}
//	}
//}
//
//void Physics::SetGlobalRestitution(Point restCoeficient)
//{
//	if (restCoeficient == Point{ 0.0f, 0.0f }) globalRestitutionActive = false;
//	else
//	{
//		globalRestitutionActive = true;
//		globalRestitution = restCoeficient;
//	}
//}
//
//void Physics::ChangeRestitutionCoeficient(Point restCoeficient)
//{
//	std::vector<Body*>::const_iterator it;
//	for (it = bodies.begin(); it != bodies.end(); ++it)
//	{
//		Body* body = (*it);
//		switch (body->bodyType)
//		{
//		case BodyType::DYNAMIC_BODY:
//			DynamicBody* dB = (DynamicBody*)body;
//			dB->coeficientRestitution = restCoeficient;
//			break;
//		}
//	}
//}
//
//void Physics::SetGlobalFriction(Point frictionCoeficient)
//{
//	if (frictionCoeficient == Point{ 0.0f, 0.0f }) globalFrictionActive = false;
//	else
//	{
//		globalFrictionActive = true;
//		globalFriction = frictionCoeficient;
//	}
//}
//
//void Physics::ChangeFrictionCoeficient(Point frictCoeficient)
//{
//	std::vector<Body*>::const_iterator it;
//	for (it = bodies.begin(); it != bodies.end(); ++it)
//	{
//		Body* body = (*it);
//		switch (body->bodyType)
//		{
//		case BodyType::DYNAMIC_BODY:
//			DynamicBody* dB = (DynamicBody*)body;
//			dB->frictionCoeficient = frictCoeficient;
//			break;
//		}
//	}
//}
//
//void Physics::SetPhysicsPreset(PhysicsPreset phPreset)
//{
//	switch (phPreset)
//	{
//	case PhysicsPreset::PLATFORMER_PHYSICS_PRESET:
//		SetGlobalGravity(Point{ 0.0f, 10.0f });
//		SetGlobalRestitution(Point{ 0.02f, 0.0f });
//		SetGlobalFriction(Point{0.1f, 0.0f});
//		break;
//
//	case PhysicsPreset::MOON_PHYSICS_PRESET:
//		SetGlobalGravity(Point{ 0.0f, 1.62f });
//		SetGlobalRestitution(Point{ 1.0f, 1.0f });
//		SetGlobalFriction(Point{ 0.02f, 0.0f });
//		break;
//
//	case PhysicsPreset::SPACE_PHYSICS_PRESET:
//		SetGlobalGravity(Point{ 0.0f, 0.0f });
//		SetGlobalRestitution(Point{ 1.0f, 1.0f });
//		SetGlobalFriction(Point{ 0.0f, 0.0f });
//		break;
//	}
//}
//
//void Physics::DeathLimit(Rect limits)
//{
//	std::vector<Body*>::const_iterator it;
//	for (it = bodies.begin(); it != bodies.end(); ++it)
//	{
//		Body* body = (*it);
//		switch (body->colliderType)
//		{
//		case CollisionType::RECTANGLE:
//			if (!MathUtils::CheckCollision(body->rect, limits)) DestroyBody(it);
//			break;
//
//		case CollisionType::CIRCLE:
//			if (!MathUtils::CheckCollision(body->circle, limits)) DestroyBody(it);
//			break;
//		}
//	}
//}
//
//void Physics::PausePhysics(bool pause)
//{
//	globalPause = pause;
//	collidingPlayer = nullptr;
//}
//
//void Physics::ResetAllForces()
//{
//	std::vector<Body*>::const_iterator it;
//	for (it = bodies.begin(); it != bodies.end(); ++it)
//	{
//		Body* body = (*it);
//		switch (body->bodyType)
//		{
//		case BodyType::DYNAMIC_BODY:
//			DynamicBody* dB = (DynamicBody*)body;
//			dB->acceleration.Zero();
//			dB->velocity.Zero();
//			dB->sumForces.Zero();
//			dB->forces.Clear();
//			break;
//		}
//	}
//}
//bool Physics::SetBodyAsPlayer(Body* b)
//{
//	std::vector<Body*>::const_iterator it;
//	for (it = bodies.begin(); it != bodies.end(); ++it) if ((*it)->player) return false;
//
//	b->player = true;
//	return true;
//}

void Physics::CheckCollisions(Body* b, BodyBackUp backup)
{
	std::vector<Body*> ghostColliders;
	DynArray<GhostSlot> slotList;

	std::vector<Body*>::const_iterator it;
	for (it = bodies.begin(); it != bodies.end(); ++it)
	{
		Body* body = (*it);
		if (body == b) continue;
		
		if (MathUtils::CheckCollision(body->rect, b->rect)) ghostColliders.push_back(body);
	}

	if (ghostColliders.size() != 0)
	{
		short int i = 0;
		for (it = ghostColliders.begin(); it != ghostColliders.end(); ++it)
		{
			Body* body = (*it);
			Rect inter = MathUtils::IntersectRectangle(b->rect, body->rect);
			Direction dir = (Direction)DirectionDetection(b->GetPosition(), backup.rect.GetPosition());
			if (inter.h < 1) inter.h = 1;
			if (inter.w < 1) inter.w = 1;
			slotList.PushBack({ dir, (int)(inter.w * inter.h), i });
			i++;
		}

		if (slotList.Count() > 2)
		{
			slotList.CombSort();
			slotList.Flip();
		}
		if (slotList.Count() == 2)
		{
			if (slotList[0] < slotList[1]) slotList.Flip();
		}

		Direction invDir = (Direction)InvertDirection((int)slotList[0].dir);

		//bodyList1->data->SolveCollision(*b, invDir);
		b->SolveCollision(*ghostColliders.at(slotList[0].slot), (int)slotList[0].dir);

		//if (b->player) return ghostColliders.at(slotList[0].slot);
		
		return;
	}
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

//void DynamicBody::SetGravityAcceleration(Point& gravity)
//{
//	this->gravityAcceleration = gravity;
//}
//
//void DynamicBody::SetConstantVelocity(Point& constVelocity)
//{
//	this->velocity = velocity;
//}
//
//void DynamicBody::Move(float newtons, Direction dir, float velocityLimiter)
//{
//	newtons = abs(newtons);
//	velocityLimiter = abs(velocityLimiter);
//	bool vL = (velocityLimiter == 0.0f);
//
//	switch (dir)
//	{
//	case Direction::UP:
//		if (vL) this->ApplyForce({ 0.0f, -newtons });
//		else
//		{
//			if (velocity.y >= -velocityLimiter) this->ApplyForce({ 0.0f, -newtons });
//		}
//		break;
//
//	case Direction::DOWN:
//		if (vL) this->ApplyForce({ 0.0f, newtons });
//		else
//		{
//			if (velocity.y <= velocityLimiter) this->ApplyForce({ 0.0f, newtons });
//		}
//		break;
//
//	case Direction::LEFT:
//		if (vL) this->ApplyForce({ -newtons, 0.0f });
//		else
//		{
//			if (velocity.x >= -velocityLimiter) this->ApplyForce({ -newtons, 0.0f });
//		}
//		break;
//
//	case Direction::RIGHT:
//		if (vL) this->ApplyForce({ newtons, 0.0f });
//		else
//		{
//			if (velocity.x <= velocityLimiter) this->ApplyForce({ newtons, 0.0f });
//		}
//		break;
//	}
//}
//
//void DynamicBody::Dash(float moment, DashDirection dDir, uint dashLimit)
//{
//	if (!onGround && !onLeftWall && !onRightWall && !onRoof) //if (onAir)
//	{
//		if (dashLimit == 0 || dashLimit > dashCount)
//		{
//			if (dashLimit != 0) dashCount++;
//
//			this->onDash = true;
//			this->onJump = false;
//			this->onDoubleJump = false;
//			this->onWallJump = false;
//
//			moment = abs(moment);
//
//			switch (dDir)
//			{
//			case DashDirection::UP:
//				this->ApplyMomentum({ 0.0f, -moment });
//				break;
//
//			case DashDirection::DOWN:
//				this->ApplyMomentum({ 0.0f, moment });
//				break;
//
//			case DashDirection::LEFT:
//				this->ApplyMomentum({ -moment, 0.0f });
//				break;
//
//			case DashDirection::RIGHT:
//				this->ApplyMomentum({ moment, 0.0f });
//				break;
//
//			case DashDirection::UPLEFT:
//				this->ApplyMomentum({ -moment, -moment });
//				break;
//
//			case DashDirection::DOWNLEFT:
//				this->ApplyMomentum({ -moment, moment });
//				break;
//
//			case DashDirection::UPRIGHT:
//				this->ApplyMomentum({ moment, -moment });
//				break;
//
//			case DashDirection::DOWNRIGHT:
//				this->ApplyMomentum({ moment, moment });
//				break;
//			}
//		}
//	}
//}
//
//void DynamicBody::Jump(float momentY, bool doubleJump)
//{
//	momentY = abs(momentY) * -1;
//
//	// DOUBLE JUMPOOO
//	if (doubleJump && !onDoubleJump && !onWallJump && onJump && !onLeftWall && !onRightWall)
//	{
//		this->ApplyMomentum({ 0.0f, momentY / 2});
//		onDoubleJump = true;
//		onJump = false;
//	}
//
//	// JUMP
//	if (onGround && !onJump)
//	{
//		this->ApplyMomentum({ 0.0f, momentY });
//		onJump = true;
//	}
//}
//
//void DynamicBody::WallJump(Point newtons, bool wallFriction)
//{
//	newtons.Abs();
//	if (onLeftWall && !onGround && !onWallJump)
//	{
//		this->ApplyForce({ newtons.x, -newtons.y });
//		onWallJump = true;
//		onJump = false;
//		onDoubleJump = false;
//		onDash = false;
//	}
//
//	if (onRightWall && !onGround && !onWallJump)
//	{
//		this->ApplyForce({ -newtons.x, -newtons.y });
//		onWallJump = true;
//		onJump = false;
//		onDoubleJump = false;
//	}
//}
//
//void DynamicBody::ApplyMomentum(Point momentum)
//{
//	momentums.PushBack(momentum);
//}
//
//void DynamicBody::ApplyMomentum(float momentumX, float momentumY)
//{
//	Point momentum = {momentumX, momentumY};
//	momentums.PushBack(momentum);
//}
//
//void DynamicBody::ResetForces()
//{
//	this->acceleration.Zero();
//	this->velocity.Zero();
//	this->sumForces.Zero();
//	this->forces.Clear();
//}

//void DynamicBody::ApplyFriction(float dt)
//{
//	//Soc molt bo. Estic calculant la força que necessito per parar el cos. La redueixo i la converteixo en friction
//
//	Point dragForce = {0.0f, 0.0f};
//	dragForce.x = (velocity.x / dt) * mass * frictionCoeficient.x;
//	dragForce.y = (velocity.y / dt) * mass * frictionCoeficient.y;
//
//	dragForce.Negate();
//
//	forces.PushBack(dragForce);
//}

//void DynamicBody::ApplyBuoyancy()
//{
//	if (buoyancyActive)
//	{
//		Point buoyancyForce = this->gravityAcceleration;
//
//		float magnitude = sqrt(pow(this->gravityAcceleration.x, 2) + pow(this->gravityAcceleration.y, 2));
//
//		buoyancyForce = { buoyancyForce.x / magnitude, buoyancyForce.y / magnitude };
//		buoyancyForce.Negate();
//
//		Point buoyancyForceMagnitude = { 0,0 };
//		buoyancyForceMagnitude.x = mass * this->gravityAcceleration.x * velocity.x - mass * this->gravityAcceleration.x;
//		buoyancyForceMagnitude.y = mass * this->gravityAcceleration.y * velocity.y - mass * this->gravityAcceleration.y;
//
//		buoyancyForce.x = buoyancyForce.x * buoyancyForceMagnitude.x;
//		buoyancyForce.y = buoyancyForce.y * buoyancyForceMagnitude.y;
//
//		forces.PushBack(buoyancyForce);
//	}
//}
//
//void DynamicBody::ApplyHidroDrag()
//{
//	if (buoyancyActive)
//	{
//		Point hidroDrag = velocity;
//
//		float magnitude = sqrt(pow(this->velocity.x, 2) + pow(this->velocity.y, 2));
//
//		hidroDrag = { hidroDrag.x / magnitude, hidroDrag.y / magnitude };
//		hidroDrag.Negate();
//
//		Point hidroDragMagnitude = { 0,0 };
//		hidroDragMagnitude.x =velocity.x * this->hydroControlParameter;
//		hidroDragMagnitude.y =velocity.y * this->hydroControlParameter;
//
//		hidroDrag.x = hidroDrag.x * hidroDragMagnitude.x;
//		hidroDrag.y = hidroDrag.y * hidroDragMagnitude.y;
//
//		forces.PushBack(hidroDrag);
//	}
//}
//
//void DynamicBody::ResetBools()
//{
//	this->onGround = false;
//	this->onRoof = false;
//	this->onLeftWall = false;
//	this->onRightWall = false;
//}
//double Body::ToPositiveAngle(double angle)
//{
//	angle = fmod(angle, 360);
//	while (angle < 0) { //pretty sure this comparison is valid for doubles and floats
//		angle += 360.0;
//	}
//
//	return angle;
//}
//
//else if (currentBody->colliderType == RECTANGLE && body.colliderType == CIRCLE)
//			{
//			//TOP & DOWN
//			if ((currentBody->position.y + currentBody->rect.h >= body.circle.y - body.circle.radius) && !(currentBody->position.y >= body.circle.y - body.circle.radius) && (currentBody->position.y + currentBody->rect.h <= body.circle.y + body.circle.radius))
//			{
//				// Ground
//				currentBody->position.y = body.circle.y - body.circle.radius - currentBody->rect.h;
//				currentBody->velocity.y = -currentBody->velocity.y * currentBody->coeficientRestitution.y;
//
//				currentBody->onGround = true;
//				currentBody->dashCount = 0;
//				if (currentBody->onJump)
//				{
//					currentBody->onJump = false;
//					if (currentBody->onDoubleJump) currentBody->onDoubleJump = false;
//				}
//				if (currentBody->onDoubleJump) currentBody->onDoubleJump = false;
//				if (currentBody->onWallJump) currentBody->onWallJump = false;
//				if (currentBody->onDash) currentBody->onDash = false;
//			}
//			else if ((currentBody->position.y <= body.circle.y + body.circle.radius) && (currentBody->position.y >= body.circle.y - body.circle.radius) && !(currentBody->position.y + currentBody->rect.h <= body.position.y + body.circle.radius))
//			{
//				// Top
//				currentBody->position.y = body.circle.y + body.circle.radius;
//				currentBody->velocity.y = -currentBody->velocity.y * currentBody->coeficientRestitution.y;
//				currentBody->onRoof = true;
//			}
//
//			//LEFT & RIGHT
//			if ((currentBody->position.x <= body.circle.x + body.circle.radius) && (currentBody->position.x >= body.circle.x - body.circle.radius) && !(currentBody->position.x + currentBody->rect.w <= body.position.x + body.circle.radius))
//			{
//				// Left wall
//				currentBody->position.x = body.circle.x + body.circle.radius;
//				currentBody->velocity.x = -currentBody->velocity.x * currentBody->coeficientRestitution.x;
//
//				currentBody->onLeftWall = true;
//				if (currentBody->onWallJump) currentBody->onWallJump = false;
//			}
//			else if ((currentBody->position.x + currentBody->rect.w >= body.circle.x - body.circle.radius) && !(currentBody->position.x >= body.circle.x - body.circle.radius) && (currentBody->position.x + currentBody->rect.w <= body.circle.x + body.circle.radius))
//			{
//				// Right wall
//				currentBody->position.x = body.circle.x - body.circle.radius - currentBody->rect.w;
//				currentBody->velocity.x = -currentBody->velocity.x * currentBody->coeficientRestitution.x;
//				currentBody->onRightWall = true;
//				if (currentBody->onWallJump) currentBody->onWallJump = false;
//			}
//
//			//POSITION SET
//			currentBody->circle.x = (int)currentBody->position.x;
//			currentBody->circle.y = (int)currentBody->position.y;
//			}
//else if (currentBody->colliderType == CIRCLE && body.colliderType == RECTANGLE)
//			{
//			//TOP & DOWN
//			if (currentBody->position.y + currentBody->rect.h >= body.position.y && !(currentBody->position.y >= body.position.y) && (currentBody->position.y + currentBody->rect.h <= body.position.y + body.circle.GetDiameter()))
//			{
//				// Ground
//				currentBody->position.y = body.circle.y - currentBody->rect.h;
//				currentBody->velocity.y = -currentBody->velocity.y * currentBody->coeficientRestitution.y;
//
//				currentBody->dashCount = 0;
//				currentBody->onGround = true;
//				if (currentBody->onJump)
//				{
//					currentBody->onJump = false;
//					if (currentBody->onDoubleJump) currentBody->onDoubleJump = false;
//				}
//				if (currentBody->onDoubleJump) currentBody->onDoubleJump = false;
//				if (currentBody->onWallJump) currentBody->onWallJump = false;
//				if (currentBody->onDash) currentBody->onDash = false;
//			}
//			else if ((currentBody->position.y <= body.position.y + body.circle.GetDiameter()) && currentBody->position.y >= body.position.y && !(currentBody->position.y + currentBody->rect.h <= body.position.y + body.circle.GetDiameter()))
//			{
//				// Top
//				currentBody->position.y = body.circle.y + body.circle.radius;
//				currentBody->velocity.y = -currentBody->velocity.y * currentBody->coeficientRestitution.y;
//				currentBody->onRoof = true;
//			}
//
//			//LEFT & RIGHT
//			if ((currentBody->position.x <= body.position.x + body.circle.GetDiameter()) && currentBody->position.x >= body.position.x && !(currentBody->position.x + currentBody->rect.w <= body.position.x + body.circle.GetDiameter()))
//			{
//				// Left wall
//				currentBody->position.x = body.circle.x + body.circle.radius;
//				currentBody->velocity.x = -currentBody->velocity.x * currentBody->coeficientRestitution.x;
//				currentBody->onLeftWall = true;
//				if (currentBody->onWallJump) currentBody->onWallJump = false;
//			}
//			else if ((currentBody->position.x + currentBody->rect.w >= body.position.x) && !(currentBody->position.x >= body.position.x))
//			{
//				// Right wall
//				currentBody->position.x = body.circle.x - currentBody->rect.w;
//				currentBody->velocity.x = -currentBody->velocity.x * currentBody->coeficientRestitution.x;
//				currentBody->onRightWall = true;
//				if (currentBody->onWallJump) currentBody->onWallJump = false;
//			}
//
//			//POSITION SET
//			currentBody->rect.x = (int)currentBody->position.x;
//			currentBody->rect.y = (int)currentBody->position.y;
//			}
//else if (currentBody->colliderType == CIRCLE && body.colliderType == CIRCLE)
//			{
//			//TOP & DOWN
//			if (currentBody->position.y + currentBody->circle.GetDiameter() >= body.position.y && !(currentBody->position.y >= body.position.y) && (currentBody->position.y + currentBody->circle.GetDiameter() <= body.position.y + body.circle.GetDiameter()))
//			{
//				// Ground
//				currentBody->position.y = body.circle.y - currentBody->circle.radius;
//				currentBody->velocity.y = -currentBody->velocity.y * currentBody->coeficientRestitution.y;
//
//				currentBody->onGround = true;
//				currentBody->dashCount = 0;
//				if (currentBody->onJump)
//				{
//					currentBody->onJump = false;
//					if (currentBody->onDoubleJump) currentBody->onDoubleJump = false;
//				}
//				if (currentBody->onDoubleJump) currentBody->onDoubleJump = false;
//				if (currentBody->onWallJump) currentBody->onWallJump = false;
//				if (currentBody->onDash) currentBody->onDash = false;
//			}
//			else if ((currentBody->position.y <= body.position.y + body.circle.GetDiameter()) && currentBody->position.y >= body.position.y && !(currentBody->position.y + currentBody->circle.GetDiameter() <= body.position.y + body.circle.GetDiameter()))
//			{
//				// Top
//				currentBody->position.y = body.circle.y + body.circle.radius;
//				currentBody->velocity.y = -currentBody->velocity.y * currentBody->coeficientRestitution.y;
//				currentBody->onRoof = true;
//			}
//
//			//LEFT & RIGHT
//			if ((currentBody->position.x <= body.position.x + body.circle.GetDiameter()) && currentBody->position.x >= body.position.x && !(currentBody->position.x + currentBody->circle.GetDiameter() <= body.position.x + body.circle.GetDiameter()))
//			{
//				// Left wall
//				currentBody->position.x = body.circle.x + currentBody->circle.radius;
//				currentBody->velocity.x = -currentBody->velocity.x * currentBody->coeficientRestitution.x;
//				currentBody->onLeftWall = true;
//				if (currentBody->onWallJump) currentBody->onWallJump = false;
//			}
//			else if ((currentBody->position.x + currentBody->circle.GetDiameter() >= body.position.x) && !(currentBody->position.x >= body.position.x))
//			{
//				// Right wall
//				currentBody->position.x = body.circle.x - currentBody->circle.radius;
//				currentBody->velocity.x = -currentBody->velocity.x * currentBody->coeficientRestitution.x;
//				currentBody->onRightWall = true;
//				if (currentBody->onWallJump) currentBody->onWallJump = false;
//			}
//
//			//POSITION SET
//			currentBody->circle.x = (int)currentBody->position.x;
//			currentBody->circle.y = (int)currentBody->position.y;
//			}