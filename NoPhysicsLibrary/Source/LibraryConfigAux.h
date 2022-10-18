#pragma once

#include "MathUtils.h"
#include "Body.h"

struct LibraryConfig
{
	LibraryConfig(float* panRange, Flag* physicsConfig, Flag* bodiesConfig, Point* globalGravity, Point* globalRestitution, Point* globalFriction, Body** listener, float* pixelsToMeters, float* ptmRatio, ScenarioPreset* scenarioPreset, Point* windowSize, PhysicsPreset* physicsPreset, Flag* notifier) :
		panRange(panRange),
		physicsConfig(physicsConfig),
		bodiesConfig(bodiesConfig),
		globalGravity(globalGravity),
		globalRestitution(globalRestitution),
		globalFriction(globalFriction),
		listener(listener),
		pixelsToMeters(pixelsToMeters),
		ptmRatio(ptmRatio),
		scenarioPreset(scenarioPreset),
		windowSize(windowSize),
		physicsPreset(physicsPreset),
		notifier(notifier)
	{}

	// Set which distance (in meters) the audio will sound mono in one of the two channels (Left / Right)
	// Remember to configure PixelsToMeters before configuring the panning range
	void PanRange(float panRange, InUnit unit) 
	{ 
		if (unit == InUnit::IN_PIXELS) panRange *= *pixelsToMeters;
		*this->panRange = panRange > 0 ? panRange : 0; 
	}

	// Allows to debug body collisions. If enabled, "GetCollisionsIterable()" inside NPL class will no longer return null. 
	// Then draw the rectangle inside it. Enabling this might slightly slow the code iteration
	void CollisionsDebugging(bool enable) { this->physicsConfig->Set(0, enable); }

	// Allows to debug body state of collision. If enabled, "IsColliding()" in DynamicBody class will no longer return false.
	// will no longer return false. Enabling this might slightly slow the code iteration
	void StateDebugging(bool enable) { this->bodiesConfig->Set(0, enable); }

	void GlobalGravity(Point gravity, InUnit unit)
	{
		if (unit == InUnit::IN_PIXELS) gravity *= *pixelsToMeters;
		*globalGravity = gravity;
	}
	void GlobalGravity(float gravity, InUnit unit)
	{
		if (unit == InUnit::IN_PIXELS) gravity *= *pixelsToMeters;
		*globalGravity = gravity;
	}

	// Restitution is a coeficient that can not be negative.
	// More than 1 stablishes a higher kinematic energy after collision
	// Invalid values are setted to 0
	void GlobalRestitution(Point restitution)
	{
		if (restitution.x < 0) restitution.x = 0;
		if (restitution.y < 0) restitution.y = 0;
		*globalRestitution = restitution;
	}
	void GlobalRestitution(float restitution)
	{
		if (restitution < 0 && restitution > 1) restitution = 0;
		*globalRestitution = restitution;
	}

	// Friction is a coeficient that lies between 0 and 1
	// Invalid values are setted to 1
	void GlobalFriction(Point friction)
	{
		if (friction.x < 0 && friction.x > 1) friction.x = 1;
		if (friction.y < 0 && friction.y > 1) friction.y = 1;
		*globalFriction = friction;
	}
	void GlobalFriction(float friction)
	{
		if (friction < 0 && friction > 1) friction = 0;
		*globalFriction = friction;
	}

	void Listener(Body* listener) { *this->listener = listener; }

	void PixelsToMeters(float ratio)
	{
		float old = *this->pixelsToMeters;
		*this->pixelsToMeters = ratio > 0 ? 1 / ratio : 1;
		*this->ptmRatio = *this->pixelsToMeters / old;
		notifier->Set(0, true);
	}

	void Scenario(ScenarioPreset preset, Point windowSize)
	{
		if (preset == ScenarioPreset::NO_SCENARIO_PRESET) return;

		*this->scenarioPreset = preset;
		*this->windowSize = windowSize;
		notifier->Set(1, true);
	}

	void Physics(PhysicsPreset preset)
	{
		*this->physicsPreset = preset;
		notifier->Set(2, true);
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
	ScenarioPreset* scenarioPreset = nullptr;
	Point* windowSize = nullptr;
	PhysicsPreset* physicsPreset = nullptr;

	Flag* notifier = nullptr;
};
