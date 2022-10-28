#pragma once

#include "NoPhysicsLibrary.h"
#include "Rect.h"

class GetData
{
private:
	
	GetData(std::vector<Body*>* bodies, std::vector<Collision*>* collisions, Flag* physicsConfig, Point* globalGravity, Point* globalFriction, Point* globalRestitution, Flag* physicsGlobals, float* pixelsToMeters) :
		bodies(bodies),
		collisions(collisions),
		physicsConfig(physicsConfig),
		globalGravity(globalGravity),
		globalFriction(globalFriction),
		globalRestitution(globalRestitution),
		physicsGlobals(physicsGlobals),
		pixelsToMeters(pixelsToMeters)
	{}

	const GetData* Access() const
	{
		return this;
	}

	friend class NPL;

public:

	unsigned int BodiesCount() const { return bodies->size(); }

	const Body* Bodies(int index) const 
	{
		if (index > BodiesCount() - 1 || index < 0) return nullptr;

		return bodies->at(index);
	}

	unsigned int CollisionsCount() const { return collisions->size(); }

	const Collision* Collisions(int index) const 
	{
		if (!physicsConfig->Get(0)) return nullptr;

		if (index > CollisionsCount() - 1 || index < 0) return nullptr;

		return collisions->at(index);
	}

	// Returns the global gravity vector
	Point GlobalGravity() const { return *globalGravity; }

	// Returns the global friction vector
	Point GlobalFriction() const { return *globalFriction; }

	// Returns the global restitution vector
	Point GlobalRestitution() const { return *globalRestitution; }

	// Returns wether the physics are paused
	bool GlobalPause() const { return physicsGlobals->Get(0); }

	// Calculates & returns a rectsngle that englobes all the existent bodies
	const Rect ScenarioRectangle() const
	{
		Rect first = bodies->front()->GetRect(InUnit::IN_PIXELS);
		Point minP = { first.x, first.y };
		Point maxP = { first.x + first.w, first.y + first.h };

		for (Body* body : *bodies)
		{
			Rect bodyRect = body->GetRect(InUnit::IN_PIXELS);
			if (bodyRect.x + bodyRect.w > maxP.x) maxP.x = bodyRect.x + bodyRect.w;
			if (bodyRect.y + bodyRect.h > maxP.y) maxP.y = bodyRect.y + bodyRect.h;
			if (bodyRect.x < minP.x) minP.x = bodyRect.x;
			if (bodyRect.y < minP.y) minP.y = bodyRect.y;
		}

		//-TODO: iterate this accounting if the notifier of scenario preset is active!

		return Rect{ minP.x, minP.y, maxP.x - minP.x, maxP.y - minP.y };
	}

	const float PixelsToMeters() const { return *pixelsToMeters; }
	
	const float MetersToPixels() const { return 1 / *pixelsToMeters; }

private:

	std::vector<Body*>* bodies = nullptr;
	std::vector<Collision*>* collisions = nullptr;
	Flag* physicsConfig = nullptr;
	Flag* physicsGlobals = nullptr;
	Point* globalGravity = nullptr;
	Point* globalFriction = nullptr;
	Point* globalRestitution = nullptr;
	float* pixelsToMeters = nullptr;

};

