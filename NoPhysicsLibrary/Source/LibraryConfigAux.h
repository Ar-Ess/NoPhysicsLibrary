#pragma once

#include "Body.h"
#include "ScenarioPresetEnum.h"
#include "PhysicsPresetEnum.h"
#include "PhysAction.h"

struct LibraryConfig
{
private:

	LibraryConfig(float* panRange, float* panFactor, Flag* physicsConfig, PhysVec* globalGravity, PhysVec* globalRestitution, PhysVec* globalFriction, float* pixelsToMeters, float* ptmRatio, unsigned int* physIterations, PhysAction<unsigned int, PhysID>* notifier) :
		panRange(panRange),
		panFactor(panFactor),
		physicsConfig(physicsConfig),
		globalGravity(globalGravity),
		globalRestitution(globalRestitution),
		globalFriction(globalFriction),
		pixelsToMeters(pixelsToMeters),
		ptmRatio(ptmRatio),
		physIterations(physIterations),
		notifier(notifier)
	{}

	const LibraryConfig* Access() const
	{
		return this;
	}
	
	friend class NPL;

public:
	// Set which distance (in meters) the audio will sound mono in one of the two channels (Left / Right)
	// Remember to configure PixelsToMeters before configuring the panning range
	void PanRange(float panRange, InUnit unit) const
	{ 
		if (unit == InUnit::IN_PIXELS) panRange *= *pixelsToMeters;
		*this->panRange = panRange > 0 ? panRange : 0; 
	}

	// Set the factor of the curve that describes the pan range
	// The default value is 1, which means a linear line.
	// The value can not be equal or less than 0
	void PanFactor(float factor) const
	{
		PhysMath::Clamp(factor, 0.01f, 20);
		*panFactor = factor;
	}

	// Allows to debug body collisions. If enabled, "GetCollisionsIterable()" inside NPL class will no longer return null. 
	// Then draw the rectangle inside it. Enabling this might slightly slow the code iteration
	void CollisionsDebugging(bool enable) const { this->physicsConfig->Set(0, enable); }

	void GlobalGravity(PhysVec gravity, InUnit unit) const
	{
		if (unit == InUnit::IN_PIXELS) gravity *= *pixelsToMeters;
		*globalGravity = gravity;
	}
	void GlobalGravity(float gravity, InUnit unit) const
	{
		if (unit == InUnit::IN_PIXELS) gravity *= *pixelsToMeters;
		*globalGravity = gravity;
	}

	// Restitution is a coefficient that can not be negative.
	// More than 1 stablishes a higher kinematic energy after collision
	// Invalid values are setted to 0
	void GlobalRestitution(PhysVec restitution) const
	{
		if (restitution.x < 0) restitution.x = 0;
		if (restitution.y < 0) restitution.y = 0;
		*globalRestitution = restitution;
	}
	void GlobalRestitution(float restitution) const
	{
		if (restitution < 0) restitution = 0;
		*globalRestitution = restitution;
	}

	// Friction is a coefficient that lies between 0 and 1
	// Invalid values are setted to 1
	void GlobalFriction(PhysVec friction) const
	{
		if (friction.x < 0 && friction.x > 1) friction.x = 1;
		if (friction.y < 0 && friction.y > 1) friction.y = 1;
		*globalFriction = friction;
	}
	void GlobalFriction(float friction) const
	{
		if (friction < 0 && friction > 1) friction = 1;
		*globalFriction = friction;
	}

	void Listener(Body* listener) const { if (listener != nullptr) notifier->Invoke(1, listener->Id()); }

	void PixelsToMeters(float ratio) const
	{
		float old = *this->pixelsToMeters;
		*this->pixelsToMeters = ratio > 0 ? 1 / ratio : 1;
		*this->ptmRatio = *this->pixelsToMeters / old;
		notifier->Invoke(0, PhysID(true)); // Pixels To Meters
	}

	// Define the amount of collision detection and declipping iterations per frame
	// The higher iterations, the better collisions will be, but more computational cost
	// Default value is set to 40
	void PhysicsIterations(unsigned int iterations) const
	{
		*physIterations = iterations;
	}

private:

	float* panRange = nullptr;
	float* panFactor = nullptr;
	Flag* physicsConfig = nullptr;
	Flag* bodiesConfig = nullptr;
	float* pixelsToMeters = nullptr;
	float* ptmRatio = nullptr;
	PhysVec* globalGravity = nullptr;
	PhysVec* globalRestitution = nullptr;
	PhysVec* globalFriction = nullptr;
	unsigned int* physIterations = nullptr;

	PhysAction<unsigned int, PhysID>* notifier = nullptr;
};
