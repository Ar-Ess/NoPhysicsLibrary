#define _CRT_SECURE_NO_WARNINGS

#include "Input.h"
#include "Textures.h"
#include "Window.h"
#include "Scene.h"
#include "Physics.h"
#include "GuiManager.h"
#include "GuiString.h"

#include "Defs.h"
#include "Log.h"

Physics::Physics(Render* render, GuiManager* gui)
{
	this->render = render;
	this->gui = gui;
}

Physics::~Physics()
{}

bool Physics::Start()
{
	gui->CreateGuiControl(GuiControlType::TEXT, {   5,  10 }, 0).TextSettings("GND:", { 255, 255, 255, 255 });
	gui->CreateGuiControl(GuiControlType::TEXT, {   5,  35 }, 0).TextSettings("LWL:", { 255, 255, 255, 255 });
	gui->CreateGuiControl(GuiControlType::TEXT, {   5,  60 }, 0).TextSettings("RWL:", { 255, 255, 255, 255 });
	gui->CreateGuiControl(GuiControlType::TEXT, {   5,  85 }, 0).TextSettings(" RF:", { 255, 255, 255, 255 });
	gui->CreateGuiControl(GuiControlType::TEXT, {   5, 110 }, 0).TextSettings("AIR:", { 255, 255, 255, 255 });
	gui->CreateGuiControl(GuiControlType::TEXT, {  85,  10 }, 0).TextSettings("JMP:", { 255, 255, 255, 255 });
	gui->CreateGuiControl(GuiControlType::TEXT, {  85,  35 }, 0).TextSettings("DJP:", { 255, 255, 255, 255 });
	gui->CreateGuiControl(GuiControlType::TEXT, {  85,  60 }, 0).TextSettings("WJP:", { 255, 255, 255, 255 });
	gui->CreateGuiControl(GuiControlType::TEXT, {  85,  85 }, 0).TextSettings("DSH:", { 255, 255, 255, 255 });
	gui->CreateGuiControl(GuiControlType::TEXT, {  85, 110 }, 0).TextSettings("WTR:", { 255, 255, 255, 255 });
	gui->CreateGuiControl(GuiControlType::TEXT, { 165,  10 }, 0).TextSettings("POS:", { 255, 255, 255, 255 });
	gui->CreateGuiControl(GuiControlType::TEXT, { 165,  35 }, 0).TextSettings("VEL:", { 255, 255, 255, 255 });
	gui->CreateGuiControl(GuiControlType::TEXT, { 165,  60 }, 0).TextSettings("ACC:", { 255, 255, 255, 255 });
	return true;
}

bool Physics::Update(float dt)
{
	if (globalPause) return true;


	if (globalGravityActive) ChangeGravityAcceleration(globalGravity);

	if (globalRestitutionActive) ChangeRestitutionCoeficient(globalRestitution);
	else ChangeRestitutionCoeficient({ 1.0f, 1.0f });

	if (globalFrictionActive) ChangeFrictionCoeficient(globalFriction);

	Step(dt);

	return true;
}

void Physics::Draw(float dt, DynamicBody* dB)
{
	if (debug) DebugDraw();
	if (debugBools) DebugBools(dB, dt);
	ResetBodyBools();
}

bool Physics::CleanUp()
{
	LOG("Freeing scene");

	std::vector<Body*>::const_iterator it;
	for (it = bodies.begin(); bodies.empty(); it) DestroyBody(it);
	
	bodies.shrink_to_fit();
	bodies.clear();

	return true;
}

void Physics::DebugDraw()
{
	std::vector<Body*>::const_iterator it;
	for (it = bodies.begin(); it != bodies.end(); ++it)
	{
		Body* body = (*it);
		Rect rectx = {};
		Rect rect = {};
		CircleCollider circx;
		CircleCollider circ;

		if (body->isCollidable)
		{
			switch (body->colliderType)
			{
			case CollisionType::RECTANGLE:
				rectx = { body->rect };
				rect = { METERS_TO_PIXELS(rectx.x), METERS_TO_PIXELS(rectx.y), METERS_TO_PIXELS(rectx.w), METERS_TO_PIXELS(rectx.h) };

				switch (body->bodyType)
				{
				case BodyType::DYNAMIC_BODY:
					render->DrawRectangle(rect, { 255, 200, 0, 150 });
					break;
				case BodyType::STATIC_BODY:
					render->DrawRectangle(rect, { 255, 0, 0, 150 });
					break;
				}
				break;

			case CollisionType::CIRCLE:
				circx = body->circle;
				circ = { (float)METERS_TO_PIXELS(circx.x), (float)METERS_TO_PIXELS(circx.y), (float)METERS_TO_PIXELS(circx.radius) };

				switch (body->bodyType)
				{
				case BodyType::DYNAMIC_BODY:
					render->DrawCircle(circ, { 255, 200, 0, 150 });
					break;
				case BodyType::STATIC_BODY:
					render->DrawCircle(circ, { 255, 0, 0, 150 });
					break;
				}
				break;
			}
		}
	}
}

void Physics::DebugBools(DynamicBody* dB, float dt)
{
	bool dynamicProps = (dB != nullptr);
	bool ground = false;
	bool roof = false;
	bool jump = false;
	bool doubleJump = false;
	bool lWall = false;
	bool rWall = false;
	bool wallJump = false;
	bool dash = false;
	bool air = false;
	bool water = false;

	if (dynamicProps) render->DrawRectangle({ 0, 0, 360, 135 }, {180, 180, 180, 200}, true, false);
	else render->DrawRectangle({ 0, 0, 180, 135 }, { 180, 180, 180, 200 }, true, false);

	if (dynamicProps)
	{
		char posStr[40] = {};
		sprintf(posStr, "POS: %.2f %.2f", dB->position.x, dB->position.y);
		gui->ChangeString(10, posStr);

		char velStr[40] = {};
		sprintf(velStr, "VEL: %.2f %.2f", dB->velocity.x, dB->velocity.y);
		gui->ChangeString(11, velStr);

		char accStr[40] = {};
		float kgs = dB->mass / dt;
		sprintf(accStr, "ACC: %.2f %.2f", (dB->velocity.x * kgs), (dB->velocity.y * kgs));
		gui->ChangeString(12, accStr);
	}

	int xOff = 52;
	int xOff1 = xOff + 80;
	int yOff = 2;
	std::vector<Body*>::const_iterator it;
	for (it = bodies.begin(); it != bodies.end(); ++it)
	{
		Body* body = (*it);
		if (body->bodyType == BodyType::DYNAMIC_BODY)
		{
			DynamicBody* dB = (DynamicBody*)body;
			if (dB->onGround && !ground)
			{
				render->DrawRectangle({ (float)xOff, 10.0f - (float)yOff, 25, 15 }, { 255, 0, 0, 255 }, true, false);
				ground = true;
			}
			
			if (dB->onLeftWall && !lWall)
			{
				render->DrawRectangle({ (float)xOff, 35 - (float)yOff, 25, 15 }, { 0, 255, 0, 255 }, true, false);
				lWall = true;
			}

			if (dB->onRightWall && !rWall)
			{
				render->DrawRectangle({ (float)xOff, 60 - (float)yOff, 25, 15 }, { 255, 255, 0, 255 }, true, false);
				rWall = true;
			}
			
			if (dB->onRoof && !roof)
			{
				render->DrawRectangle({ (float)xOff, 85 - (float)yOff, 25, 15 }, { 0, 0, 255, 255 }, true, false);
				roof = true;
			}

			if (dB->onAir && !air)
			{
				render->DrawRectangle({ (float)xOff, 110 - (float)yOff, 25, 15 }, { 225, 105, 55, 255 }, true, false);
				air = true;
			}

			if (dB->onJump && !jump)
			{
				render->DrawRectangle({ (float)xOff1, 10 - (float)yOff, 25, 15 }, { 0, 255, 255, 255 }, true, false);
				jump = true;
			}

			if (dB->onDoubleJump && !doubleJump)
			{
				render->DrawRectangle({ (float)xOff1, 35 - (float)yOff, 25, 15 }, { 255, 0, 255, 255 }, true, false);
				doubleJump = true;
			}

			if (dB->onWallJump && !wallJump)
			{
				render->DrawRectangle({ (float)xOff1, 60 - (float)yOff, 25, 15 }, { 255, 255, 255, 255 }, true, false);
				wallJump = true;
			}

			if (dB->onDash && !dash)
			{
				render->DrawRectangle({ (float)xOff1, 85 - (float)yOff, 25, 15 }, { 225, 55, 105, 255 }, true, false);
				dash = true;
			}

			if (dB->onWater && !water)
			{
				render->DrawRectangle({ (float)xOff1, 110 - (float)yOff, 25, 15 }, { 55, 225, 105, 255 }, true, false);
				water = true;
			}
		}
	}
}

void Physics::ResetBodyBools()
{
	std::vector<Body*>::const_iterator it;
	for (it = bodies.begin(); it != bodies.end(); ++it)
	{
		Body* body = (*it);
		if (body->bodyType == BodyType::DYNAMIC_BODY)
		{
			DynamicBody* dB = (DynamicBody*)body;
			dB->ResetBools();
		}
	}
}

void Physics::Integrate(DynamicBody* item, float dt)
{
	// Sum of all acceleration in both axis
	// sum of all forces
	// second law newton (divide by mass) and gett total acceleration

	item->position.x += item->velocity.x * dt + 0.5f * item->acceleration.x * dt * dt;
	item->velocity.x += item->acceleration.x * dt;

	item->position.y += item->velocity.y * dt + 0.5f * item->acceleration.y * dt * dt;
	item->velocity.y += item->acceleration.y * dt;
}

void Physics::Step(float dt)
{
	std::vector<Body*>::const_iterator it;
	for (it = bodies.begin(); it != bodies.end(); ++it)
	{
		Body* body = (*it);
		switch (body->bodyType)
		{
		case BodyType::DYNAMIC_BODY:
			DynamicBody* dB = (DynamicBody*)body;
			// Correct angles rotation
			// dB->rotation = dB->ToPositiveAngle(dB->rotation);

			//dynamicBody->ApplyHidroDrag();
			//dynamicBody->ApplyBuoyancy();
			if (globalFrictionActive) dB->ApplyFriction(dt);
			//dynamicBody->ApplyAeroLift();

			// Second law newton
			dB->SecondNewton();

			// First law Buxeda
			dB->FirstBuxeda();

			// Applying gravity acceleration post second law newton
			dB->acceleration += dB->gravityAcceleration;

			Point prevPosition = dB->position;

			// Integrate
			Integrate(dB, dt);

			// 

			dB->acceleration = { 0.0f,0.0f };

			// Setting rect collider position
			switch (dB->colliderType)
			{
			case CollisionType::RECTANGLE:
				dB->rect.x = (int)dB->position.x;
				dB->rect.y = (int)dB->position.y;
				break;

			case CollisionType::CIRCLE:
				dB->circle.x = (int)dB->position.x;
				dB->circle.y = (int)dB->position.y;
				break;
			}

			// Check Colls
			collidingPlayer = CheckCollisions(dB, prevPosition);

			// onAir check
			if (!dB->onGround && !dB->onLeftWall && !dB->onRightWall && !dB->onRoof && !dB->onJump && !dB->onDoubleJump && !dB->onDash && !dB->onWallJump && !dB->onWater) dB->onAir = true;
			else dB->onAir = false;
			break;
		}
	}
}

void Physics::SetGlobalGravity(Point gravity)
{
	if (gravity == Point{ 0.0f, 0.0f }) globalGravityActive = false;
	else
	{
		globalGravityActive = true;
		globalGravity = gravity;
	}
}

void Physics::ChangeGravityAcceleration(Point acceleration)
{
	std::vector<Body*>::const_iterator it;
	for (it = bodies.begin(); it != bodies.end(); ++it)
	{
		Body* body = (*it);
		Point acc = acceleration;
		switch (body->bodyType)
		{
		case BodyType::DYNAMIC_BODY:
			DynamicBody* dB = (DynamicBody*)body;
			acc *= REALITY_MULTIPLIER;
			dB->gravityAcceleration = acc;
			break;
		}
	}
}

void Physics::SetGlobalRestitution(Point restCoeficient)
{
	if (restCoeficient == Point{ 0.0f, 0.0f }) globalRestitutionActive = false;
	else
	{
		globalRestitutionActive = true;
		globalRestitution = restCoeficient;
	}
}

void Physics::ChangeRestitutionCoeficient(Point restCoeficient)
{
	std::vector<Body*>::const_iterator it;
	for (it = bodies.begin(); it != bodies.end(); ++it)
	{
		Body* body = (*it);
		switch (body->bodyType)
		{
		case BodyType::DYNAMIC_BODY:
			DynamicBody* dB = (DynamicBody*)body;
			dB->coeficientRestitution = restCoeficient;
			break;
		}
	}
}

void Physics::SetGlobalFriction(Point frictionCoeficient)
{
	if (frictionCoeficient == Point{ 0.0f, 0.0f }) globalFrictionActive = false;
	else
	{
		globalFrictionActive = true;
		globalFriction = frictionCoeficient;
	}
}

void Physics::ChangeFrictionCoeficient(Point frictCoeficient)
{
	std::vector<Body*>::const_iterator it;
	for (it = bodies.begin(); it != bodies.end(); ++it)
	{
		Body* body = (*it);
		switch (body->bodyType)
		{
		case BodyType::DYNAMIC_BODY:
			DynamicBody* dB = (DynamicBody*)body;
			dB->frictionCoeficient = frictCoeficient;
			break;
		}
	}
}

void Physics::SetPhysicsPreset(PhysicsPreset phPreset)
{
	switch (phPreset)
	{
	case PhysicsPreset::PLATFORMER_PHYSICS_PRESET:
		SetGlobalGravity(Point{ 0.0f, 10.0f });
		SetGlobalRestitution(Point{ 0.02f, 0.0f });
		SetGlobalFriction(Point{0.1f, 0.0f});
		break;

	case PhysicsPreset::MOON_PHYSICS_PRESET:
		SetGlobalGravity(Point{ 0.0f, 1.62f });
		SetGlobalRestitution(Point{ 1.0f, 1.0f });
		SetGlobalFriction(Point{ 0.02f, 0.0f });
		break;

	case PhysicsPreset::SPACE_PHYSICS_PRESET:
		SetGlobalGravity(Point{ 0.0f, 0.0f });
		SetGlobalRestitution(Point{ 1.0f, 1.0f });
		SetGlobalFriction(Point{ 0.0f, 0.0f });
		break;
	}
}

void Physics::SetScenarioPreset(ScenarioPreset sPreset)
{
	DestroyScenario();

	switch (sPreset)
	{
	case ScenarioPreset::LIMITS_1280x720_SCENARIO_PRESET:
		//Limits
		CreateBody(BodyType::STATIC_BODY, Point{ 0, 0 }, { 0, 0, 1280, 50 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, Point{ 0, 0 }, { 0, 0, 50, 720 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, Point{ 0, 670 }, { 0, 670, 1280, 50 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, Point{ 1230, 0 }, { 1230, 0, 50, 720 }, { 0, 0 }, { 0, 0 }, 1);
		debugBools = false;
		break;

	case ScenarioPreset::PLATFORMER_1280x720_SCENARIO_PRESET:
		//Limits
		CreateBody(BodyType::STATIC_BODY, Point{ 0, 0 }, { 0, 0, 1280, 50 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, Point{ 0, 0 }, { 0, 0, 50, 720 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, Point{ 0, 670 }, { 0, 670, 1280, 50 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, Point{ 1230, 0 }, { 1230, 0, 50, 720 }, { 0, 0 }, { 0, 0 }, 1);
		//Obstacles
		CreateBody(BodyType::STATIC_BODY, Point{ 340, 490 }, { 340, 490, 600, 25 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, Point{ 540, 220 }, { 540, 220, 200, 25 }, { 0, 0 }, { 0, 0 }, 1);
		debugBools = false;
		break;

	case ScenarioPreset::WALLJUMP_1280x720_SCENARIO_PRESET:
		//Limits
		CreateBody(BodyType::STATIC_BODY, Point{ 0, 0 }, { 0, 0, 1280, 50 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, Point{ 0, 0 }, { 0, 0, 50, 720 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, Point{ 0, 670 }, { 0, 670, 1280, 50 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, Point{ 1230, 0 }, { 1230, 0, 50, 720 }, { 0, 0 }, { 0, 0 }, 1);
		//Obstacles
		CreateBody(BodyType::STATIC_BODY, Point{ 200, 200 }, { 200, 200, 25, 400 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, Point{ 500, 200 }, { 500, 200, 25, 400 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, Point{ 780, 200 }, { 780, 200, 25, 400 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, Point{ 1080, 200 }, { 1080, 200, 25, 400 }, { 0, 0 }, { 0, 0 }, 1);
		debugBools = false;
		break;

	case ScenarioPreset::SPHERE_1280x720_SCENARIO_PRESET:
		//Limits
		CreateBody(BodyType::STATIC_BODY, Point{ 0, 0 }, { 0, 0, 1280, 50 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, Point{ 0, 0 }, { 0, 0, 50, 720 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, Point{ 0, 670 }, { 0, 670, 1280, 50 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, Point{ 1230, 0 }, { 1230, 0, 50, 720 }, { 0, 0 }, { 0, 0 }, 1);
		//Sphere
		CreateBody(BodyType::STATIC_BODY, Point{ 640, 360 }, CircleCollider{ 640, 390, 120}, { 0, 0 }, { 0, 0 }, 1);
		debugBools = false;
		break;
	}
}

void Physics::DeathLimit(Rect limits)
{
	std::vector<Body*>::const_iterator it;
	for (it = bodies.begin(); it != bodies.end(); ++it)
	{
		Body* body = (*it);
		switch (body->colliderType)
		{
		case CollisionType::RECTANGLE:
			if (!utils.CheckCollision(body->rect, limits)) DestroyBody(it);
			break;

		case CollisionType::CIRCLE:
			if (!utils.CheckCollision(body->circle, limits)) DestroyBody(it);
			break;
		}
	}
}

void Physics::DestroyScenario()
{
	std::vector<Body*>::const_iterator it;
	for (it = bodies.begin(); it != bodies.end(); ++it)
	{
		Body* body = (*it);
		if (body->bodyType == BodyType::STATIC_BODY) DestroyBody(it);
	}
	return;
}

void Physics::PausePhysics(bool pause)
{
	globalPause = pause;
	collidingPlayer = nullptr;
}

void Physics::ResetAllForces()
{
	std::vector<Body*>::const_iterator it;
	for (it = bodies.begin(); it != bodies.end(); ++it)
	{
		Body* body = (*it);
		switch (body->bodyType)
		{
		case BodyType::DYNAMIC_BODY:
			DynamicBody* dB = (DynamicBody*)body;
			dB->acceleration.Zero();
			dB->velocity.Zero();
			dB->sumForces.Zero();
			dB->forces.Clear();
			break;
		}
	}
}

void Physics::DestroyBody(std::vector<Body*>::const_iterator it)
{
	Body* body = *it;
	delete body;
	bodies.erase(it);
	body = nullptr;
	bodies.shrink_to_fit();
}

bool Physics::SetBodyAsPlayer(Body* b)
{
	std::vector<Body*>::const_iterator it;
	for (it = bodies.begin(); it != bodies.end(); ++it) if ((*it)->player) return false;

	b->player = true;
	return true;
}

Body* Physics::CheckCollisions(Body* b, Point prevPos)
{
	//ListItem<Body*>* bodyList1;
	std::vector<Body*> ghostColliders;
	DynArray<GhostSlot> slotList;

	std::vector<Body*>::const_iterator it;
	for (it = bodies.begin(); it != bodies.end(); ++it)
	{
		Body* body = (*it);
		if (body != b)
		{
			if (body->colliderType == CollisionType::RECTANGLE && b->colliderType == CollisionType::RECTANGLE)
			{
				if (utils.CheckCollision(body->rect, b->rect)) ghostColliders.push_back(body);
			}
			/*else if (bodyList1->data->colliderType == CIRCLE && b->colliderType == RECTANGLE)
			{
				if (collisionUtil.CheckCollision(bodyList1->data->circle, b->rect)) collided = true;
			}
			else if (bodyList1->data->colliderType == RECTANGLE && b->colliderType == CIRCLE)
			{
				if (collisionUtil.CheckCollision(bodyList1->data->rect, b->circle)) collided = true;
			}
			else if (bodyList1->data->colliderType == CIRCLE && b->colliderType == CIRCLE)
			{
				if (collisionUtil.CheckCollision(bodyList1->data->circle, b->circle)) collided = true;
			}

			if (collided)
			{
				Direction dir = DirectionDetection(b->GetPosition(), prevPos);
				Direction invDir = InvertDirection(dir);

				bodyList1->data->SolveCollision(*b, invDir);
				b->SolveCollision(*bodyList1->data, dir);
			}*/
		}
	}

	if (ghostColliders.size() != 0)
	{
		short int i = 0;
		for (it = ghostColliders.begin(); it != ghostColliders.end(); ++it)
		{
			Body* body = (*it);
			Rect inter = utils.IntersectRectangle(b->rect, body->rect);
			Direction dir = DirectionDetection(b->GetPosition(), prevPos);
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

		Direction invDir = InvertDirection(slotList[0].dir);

		//bodyList1->data->SolveCollision(*b, invDir);
		b->SolveCollision(*ghostColliders.at(slotList[0].slot), slotList[0].dir);

		if (b->player) return ghostColliders.at(slotList[0].slot);
		
		return nullptr;
	}
}

Direction Physics::DirectionDetection(Point currPos, Point prevPos)
{
	if (prevPos.y - currPos.y < 0)
	{
		return Direction::DOWN; //COLLIDING UP TO DOWN
	}
	if (prevPos.x - currPos.x < 0)
	{
		return Direction::RIGHT; //COLLIDING LEFT TO RIGHT
	}
	if (prevPos.y - currPos.y > 0)
	{
		return Direction::UP; //COLLIDING DOWN TO UP
	}
	if (prevPos.x - currPos.x > 0)
	{
		return Direction::LEFT; //COLLIDING RIGHT TO LEFT
	}

	return Direction();
}

Direction Physics::InvertDirection(Direction dir)
{
	switch (dir)
	{
	case Direction::UP: return Direction::DOWN; break;
	case Direction::DOWN: return Direction::UP; break;
	case Direction::LEFT: return Direction::RIGHT; break;
	case Direction::RIGHT: return Direction::LEFT; break;
	}

	return Direction();
}

Body* Physics::CreateBody(BodyType bodyType, Point position, Rect rect, Point velocity, Point gravity, float mass_)
{
	Body* ret = nullptr;

	switch (bodyType)
	{
	case BodyType::DYNAMIC_BODY:
	{
		DynamicBody* newBodyD = new DynamicBody(position, velocity, gravity, CollisionType::RECTANGLE, rect, mass_);
		bodies.push_back(newBodyD);
		ret = newBodyD;
		newBodyD = nullptr;
		break;
	}

	case BodyType::STATIC_BODY:
	{
		StaticBody* newBodyS = new StaticBody(position, CollisionType::RECTANGLE, rect, mass_);
		bodies.push_back(newBodyS);
		ret = newBodyS;
		newBodyS = nullptr;
		break;
	}
	}

	return ret;
}

Body* Physics::CreateBody(BodyType bodyType_, Point position, CircleCollider circle, Point velocity, Point gravity, float mass_)
{
	Body* ret = nullptr;

	switch (bodyType_)
	{
	case BodyType::DYNAMIC_BODY:
	{
		DynamicBody* newBodyD = new DynamicBody(position, velocity, gravity, CollisionType::CIRCLE, circle, mass_);
		bodies.push_back(newBodyD);
		ret = newBodyD;
		newBodyD = nullptr;
		break;
	}

	case BodyType::STATIC_BODY:
	{
		StaticBody* newBodyS = new StaticBody(position, CollisionType::CIRCLE, circle, mass_);
		bodies.push_back(newBodyS);
		ret = newBodyS;
		newBodyS = nullptr;
		break;
	}
	}

	return ret;
}

void DynamicBody::SetGravityAcceleration(Point& gravity)
{
	this->gravityAcceleration = gravity;
}

void DynamicBody::SetConstantVelocity(Point& constVelocity)
{
	this->velocity = velocity;
}

void DynamicBody::Move(float newtons, Direction dir, float velocityLimiter)
{
	newtons = abs(newtons);
	velocityLimiter = abs(velocityLimiter);
	bool vL = (velocityLimiter == 0.0f);

	switch (dir)
	{
	case Direction::UP:
		if (vL) this->ApplyForce({ 0.0f, -newtons });
		else
		{
			if (velocity.y >= -velocityLimiter) this->ApplyForce({ 0.0f, -newtons });
		}
		break;

	case Direction::DOWN:
		if (vL) this->ApplyForce({ 0.0f, newtons });
		else
		{
			if (velocity.y <= velocityLimiter) this->ApplyForce({ 0.0f, newtons });
		}
		break;

	case Direction::LEFT:
		if (vL) this->ApplyForce({ -newtons, 0.0f });
		else
		{
			if (velocity.x >= -velocityLimiter) this->ApplyForce({ -newtons, 0.0f });
		}
		break;

	case Direction::RIGHT:
		if (vL) this->ApplyForce({ newtons, 0.0f });
		else
		{
			if (velocity.x <= velocityLimiter) this->ApplyForce({ newtons, 0.0f });
		}
		break;
	}
}

void DynamicBody::Dash(float moment, DashDirection dDir, uint dashLimit)
{
	if (!onGround && !onLeftWall && !onRightWall && !onRoof) //if (onAir)
	{
		if (dashLimit == 0 || dashLimit > dashCount)
		{
			if (dashLimit != 0) dashCount++;

			this->onDash = true;
			this->onJump = false;
			this->onDoubleJump = false;
			this->onWallJump = false;

			moment = abs(moment);

			switch (dDir)
			{
			case DashDirection::UP:
				this->ApplyMomentum({ 0.0f, -moment });
				break;

			case DashDirection::DOWN:
				this->ApplyMomentum({ 0.0f, moment });
				break;

			case DashDirection::LEFT:
				this->ApplyMomentum({ -moment, 0.0f });
				break;

			case DashDirection::RIGHT:
				this->ApplyMomentum({ moment, 0.0f });
				break;

			case DashDirection::UPLEFT:
				this->ApplyMomentum({ -moment, -moment });
				break;

			case DashDirection::DOWNLEFT:
				this->ApplyMomentum({ -moment, moment });
				break;

			case DashDirection::UPRIGHT:
				this->ApplyMomentum({ moment, -moment });
				break;

			case DashDirection::DOWNRIGHT:
				this->ApplyMomentum({ moment, moment });
				break;
			}
		}
	}
}

void DynamicBody::Jump(float momentY, bool doubleJump)
{
	momentY = abs(momentY) * -1;

	// DOUBLE JUMPOOO
	if (doubleJump && !onDoubleJump && !onWallJump && onJump && !onLeftWall && !onRightWall)
	{
		this->ApplyMomentum({ 0.0f, momentY / 2});
		onDoubleJump = true;
		onJump = false;
	}

	// JUMP
	if (onGround && !onJump)
	{
		this->ApplyMomentum({ 0.0f, momentY });
		onJump = true;
	}
}

void DynamicBody::WallJump(Point newtons, bool wallFriction)
{
	newtons.Abs();
	if (onLeftWall && !onGround && !onWallJump)
	{
		this->ApplyForce({ newtons.x, -newtons.y });
		onWallJump = true;
		onJump = false;
		onDoubleJump = false;
		onDash = false;
	}

	if (onRightWall && !onGround && !onWallJump)
	{
		this->ApplyForce({ -newtons.x, -newtons.y });
		onWallJump = true;
		onJump = false;
		onDoubleJump = false;
	}
}

void DynamicBody::ApplyForce(Point newtons)
{
	newtons *= NEWTONS_MULTIPLIER;
	forces.PushBack(newtons);
}

void DynamicBody::ApplyForce(float newtonsX, float newtonsY)
{
	Point newtons = {newtonsX, newtonsY};
	newtons *= NEWTONS_MULTIPLIER;
	forces.PushBack(newtons);
}

void DynamicBody::ApplyMomentum(Point momentum)
{
	momentums.PushBack(momentum);
}

void DynamicBody::ApplyMomentum(float momentumX, float momentumY)
{
	Point momentum = {momentumX, momentumY};
	momentums.PushBack(momentum);
}

void DynamicBody::ResetForces()
{
	this->acceleration.Zero();
	this->velocity.Zero();
	this->sumForces.Zero();
	this->forces.Clear();
}

void DynamicBody::SecondNewton()
{
	for (int i = 0; i < forces.Count(); i++)
	{
		sumForces += *forces.At(i);
		forces.Pop(*forces.At(i));
	}
	forces.Clear();

	acceleration.x += sumForces.x / mass;
	acceleration.y += sumForces.y / mass;

	sumForces = { 0.0f,0.0f };
}

void DynamicBody::FirstBuxeda()
{
	for (int i = 0; i < momentums.Count(); i++)
	{
		sumMomentum += *momentums.At(i);
		momentums.Pop(*momentums.At(i));
	}
	momentums.Clear();

	velocity.x += sumMomentum.x / mass;
	velocity.y += sumMomentum.y / mass;

	sumMomentum = { 0.0f,0.0f };
}

void DynamicBody::ApplyFriction(float dt)
{
	//Soc molt bo. Estic calculant la força que necessito per parar el cos. La redueixo i la converteixo en friction

	Point dragForce = {0.0f, 0.0f};
	dragForce.x = (velocity.x / dt) * mass * frictionCoeficient.x;
	dragForce.y = (velocity.y / dt) * mass * frictionCoeficient.y;

	dragForce.Negate();

	forces.PushBack(dragForce);
}

void DynamicBody::ApplyBuoyancy()
{
	if (buoyancyActive)
	{
		Point buoyancyForce = this->gravityAcceleration;

		float magnitude = sqrt(pow(this->gravityAcceleration.x, 2) + pow(this->gravityAcceleration.y, 2));

		buoyancyForce = { buoyancyForce.x / magnitude, buoyancyForce.y / magnitude };
		buoyancyForce.Negate();

		Point buoyancyForceMagnitude = { 0,0 };
		buoyancyForceMagnitude.x = mass * this->gravityAcceleration.x * velocity.x - mass * this->gravityAcceleration.x;
		buoyancyForceMagnitude.y = mass * this->gravityAcceleration.y * velocity.y - mass * this->gravityAcceleration.y;

		buoyancyForce.x = buoyancyForce.x * buoyancyForceMagnitude.x;
		buoyancyForce.y = buoyancyForce.y * buoyancyForceMagnitude.y;

		forces.PushBack(buoyancyForce);
	}
}

void DynamicBody::ApplyHidroDrag()
{
	if (buoyancyActive)
	{
		Point hidroDrag = velocity;

		float magnitude = sqrt(pow(this->velocity.x, 2) + pow(this->velocity.y, 2));

		hidroDrag = { hidroDrag.x / magnitude, hidroDrag.y / magnitude };
		hidroDrag.Negate();

		Point hidroDragMagnitude = { 0,0 };
		hidroDragMagnitude.x =velocity.x * this->hydroControlParameter;
		hidroDragMagnitude.y =velocity.y * this->hydroControlParameter;

		hidroDrag.x = hidroDrag.x * hidroDragMagnitude.x;
		hidroDrag.y = hidroDrag.y * hidroDragMagnitude.y;

		forces.PushBack(hidroDrag);
	}
}

void DynamicBody::ResetBools()
{
	this->onGround = false;
	this->onRoof = false;
	this->onLeftWall = false;
	this->onRightWall = false;
}

void Body::SolveCollision(Body &body, Direction dir)
{
	this->DeClipper(body, dir); // First declip, then do anything 
}

void Body::DeClipper(Body &body, Direction dir)
{
	if (body.isCollidable)
	{
		switch (this->bodyType)
		{
		case BodyType::DYNAMIC_BODY:
			DynamicBody* currentBody = (DynamicBody*)this;
			
			if (currentBody->colliderType == CollisionType::RECTANGLE && body.colliderType == CollisionType::RECTANGLE)
			{
				//TOP & DOWN & LEFT & RIGHT
				if ((currentBody->position.y + currentBody->rect.h > body.position.y) && (currentBody->position.y < body.position.y) && (currentBody->position.y + currentBody->rect.h < body.position.y + body.rect.h))
				{
					// Ground
					currentBody->position.y = body.rect.y - currentBody->rect.h;
					currentBody->velocity.y = -currentBody->velocity.y * currentBody->coeficientRestitution.y;

					currentBody->onGround = true;
					currentBody->dashCount = 0;
					if (currentBody->onJump)
					{
						currentBody->onJump = false;
						if (currentBody->onDoubleJump) currentBody->onDoubleJump = false;
					}
					if (currentBody->onDoubleJump) currentBody->onDoubleJump = false;
					if (currentBody->onWallJump) currentBody->onWallJump = false;
					if (currentBody->onDash) currentBody->onDash = false;
				}
				else if ((currentBody->position.y < body.position.y + body.rect.h) && (currentBody->position.y + currentBody->rect.h > body.position.y + body.rect.h) && (currentBody->position.y > body.position.y))
				{
					// Top
					currentBody->position.y = body.rect.y + body.rect.h;
					currentBody->velocity.y = -currentBody->velocity.y * currentBody->coeficientRestitution.y;

					currentBody->onRoof = true;
				}
				else if ((currentBody->position.x < body.position.x + body.rect.w) && (currentBody->position.x > body.position.x) && (currentBody->position.x + currentBody->rect.w > body.position.x + body.rect.w))
				{
					// Left wall
					currentBody->position.x = body.rect.x + body.rect.w;
					currentBody->velocity.x = -currentBody->velocity.x * currentBody->coeficientRestitution.x;
					currentBody->onLeftWall = true;

					if (currentBody->onWallJump) currentBody->onWallJump = false;
				}
				else if ((currentBody->position.x + currentBody->rect.w > body.position.x) && (currentBody->position.x + currentBody->rect.w < body.position.x + body.rect.w) && (currentBody->position.x < body.position.x))
				{
					// Right wall
					currentBody->position.x = body.rect.x - currentBody->rect.w;
					currentBody->velocity.x = -currentBody->velocity.x * currentBody->coeficientRestitution.x;
					currentBody->onRightWall = true;

					if (currentBody->onWallJump) currentBody->onWallJump = false;
				}

				//POSITION SET
				currentBody->rect.x = (int)currentBody->position.x;
				currentBody->rect.y = (int)currentBody->position.y;
			}

			break;
		}
	}
}

double Body::ToPositiveAngle(double angle)
{
	angle = fmod(angle, 360);
	while (angle < 0) { //pretty sure this comparison is valid for doubles and floats
		angle += 360.0;
	}

	return angle;
}

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
