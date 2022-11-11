#pragma once

#include "MathUtils.h"
#include "Body.h"
#include "ScenarioPresetEnum.h"
#include "PhysicsPresetEnum.h"

struct LibraryConfig
{
private:

	LibraryConfig(float* panRange, Flag* physicsConfig, Point* globalGravity, Point* globalRestitution, Point* globalFriction, Body** listener, float* pixelsToMeters, float* ptmRatio, Flag* notifier) :
		panRange(panRange),
		physicsConfig(physicsConfig),
		globalGravity(globalGravity),
		globalRestitution(globalRestitution),
		globalFriction(globalFriction),
		listener(listener),
		pixelsToMeters(pixelsToMeters),
		ptmRatio(ptmRatio),
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

	// Allows to debug body collisions. If enabled, "GetCollisionsIterable()" inside NPL class will no longer return null. 
	// Then draw the rectangle inside it. Enabling this might slightly slow the code iteration
	void CollisionsDebugging(bool enable) const { this->physicsConfig->Set(0, enable); }

	void GlobalGravity(Point gravity, InUnit unit) const
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
	void GlobalRestitution(Point restitution) const
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
	void GlobalFriction(Point friction) const
	{
		if (friction.x < 0 && friction.x > 1) friction.x = 1;
		if (friction.y < 0 && friction.y > 1) friction.y = 1;
		*globalFriction = friction;
	}
	void GlobalFriction(float friction) const
	{
		if (friction < 0 && friction > 1) friction = 0;
		*globalFriction = friction;
	}

	void Listener(Body* listener) const { *this->listener = listener; }

	void PixelsToMeters(float ratio) const
	{
		float old = *this->pixelsToMeters;
		*this->pixelsToMeters = ratio > 0 ? 1 / ratio : 1;
		*this->ptmRatio = *this->pixelsToMeters / old;
		notifier->Set(0, true);
	}

private:

	float* panRange = nullptr;
	Flag* physicsConfig = nullptr;
	Flag* bodiesConfig = nullptr;
	float* pixelsToMeters = nullptr;
	float* ptmRatio = nullptr;
	Point* globalGravity = nullptr;
	Point* globalRestitution = nullptr;
	Point* globalFriction = nullptr;
	Body** listener = nullptr;

	Flag* notifier = nullptr;
};
