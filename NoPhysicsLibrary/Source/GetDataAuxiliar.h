#pragma once

#include "Collision.h"

class GetData
{
private:
	
	GetData(PhysArray<Body*>* bodies, PhysArray<Collision*>* collisions, Flag* generalConfig, PhysVec* globalGravity, PhysVec* globalFriction, PhysVec* globalRestitution, Flag* physicsGlobals, float* pixelsToMeters) :
		bodies(bodies),
		collisions(collisions),
		generalConfig(generalConfig),
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

	unsigned int BodiesCount() const { return bodies->Size(); }

	const Body* Bodies(unsigned int index) const 
	{
		if (unsigned(index) > bodies->Size() - 1) return nullptr;

		return bodies->At(index);
	}

	unsigned int CollisionsCount() const { return collisions->Size(); }

	const Collision* Collisions(unsigned int index) const 
	{
		if (!generalConfig->Get(0)) return nullptr;

		if (index > collisions->Size() - 1) return nullptr;

		return collisions->At(index);
	}

	// Returns the global gravity vector
	PhysVec GlobalGravity() const { return *globalGravity; }

	// Returns the global friction vector
	PhysVec GlobalFriction() const { return *globalFriction; }

	// Returns the global restitution vector
	PhysVec GlobalRestitution() const { return *globalRestitution; }

	// Returns wether the physics are paused
	bool GlobalPause() const { return physicsGlobals->Get(0); }

	const float PixelsToMeters() const { return *pixelsToMeters; }
	
	const float MetersToPixels() const { return 1 / *pixelsToMeters; }

private:

	PhysArray<Body*>* bodies = nullptr;
	PhysArray<Collision*>* collisions = nullptr;
	Flag* generalConfig = nullptr;
	Flag* physicsGlobals = nullptr;
	PhysVec* globalGravity = nullptr;
	PhysVec* globalFriction = nullptr;
	PhysVec* globalRestitution = nullptr;
	const float* pixelsToMeters = nullptr;
};
