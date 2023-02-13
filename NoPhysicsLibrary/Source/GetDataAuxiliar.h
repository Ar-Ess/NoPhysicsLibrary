#pragma once

#include "NoPhysicsLibrary.h"
#include "Rect.h"

class GetData
{
private:
	
	GetData(std::vector<Body*>* bodies, std::vector<Collision*>* collisions, Flag* physicsConfig, PhysVec* globalGravity, PhysVec* globalFriction, PhysVec* globalRestitution, Flag* physicsGlobals, float* pixelsToMeters) :
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

	const Body* Bodies(unsigned int index) const 
	{
		if (unsigned(index) > BodiesCount() - 1) return nullptr;

		return bodies->at(index);
	}

	unsigned int CollisionsCount() const { return collisions->size(); }

	const Collision* Collisions(unsigned int index) const 
	{
		if (!physicsConfig->Get(0)) return nullptr;

		if (index > CollisionsCount() - 1) return nullptr;

		return collisions->at(index);
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

	std::vector<Body*>* bodies = nullptr;
	std::vector<Collision*>* collisions = nullptr;
	Flag* physicsConfig = nullptr;
	Flag* physicsGlobals = nullptr;
	PhysVec* globalGravity = nullptr;
	PhysVec* globalFriction = nullptr;
	PhysVec* globalRestitution = nullptr;
	float* pixelsToMeters = nullptr;

};
