#pragma once

#include "Body.h"
#include "ScenarioPresetEnum.h"
#include "PhysicsPresetEnum.h"
#include "PhysAction.h"

struct LibraryConfig
{
private:

	LibraryConfig(float* panRange, float* panFactor, float* pitchFactor, Flag* generalConfig, PhysVec* globalGravity, PhysVec* globalRestitution, PhysVec* globalFriction, float* pixelsToMeters, float* ptmRatio, unsigned int* physIterations, PhysAction<unsigned int, PhysID>* notifier, float* globalMultiplier, float* volumeAttenuationFactor) :
		panRange(panRange),
		panFactor(panFactor),
		pitchFactor(pitchFactor),
		generalConfig(generalConfig),
		globalGravity(globalGravity),
		globalRestitution(globalRestitution),
		globalFriction(globalFriction),
		pixelsToMeters(pixelsToMeters),
		ptmRatio(ptmRatio),
		physIterations(physIterations),
		notifier(notifier),
		globalMultiplier(globalMultiplier),
		volumeAttenuationFactor(volumeAttenuationFactor)
	{}

	const LibraryConfig* Access() const
	{
		return this;
	}
	
	friend class NoPhysicsLibrary;

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

	// Set the factor of the pitch variation depending on the environment
	// 1 means real variation and 0 means no variation
	// UpwardsPitch and DownwardsPitch controls if sound can be pitched up or pitched down
	void PitchVariationFactor(float factor, bool upwardsPitch, bool downwardsPitch) const
	{
		PhysMath::Clamp(factor, 0, 1);
		*pitchFactor = factor;
		generalConfig->Set(1, upwardsPitch);
		generalConfig->Set(2, downwardsPitch);
	}

	// Allows the library to calculate the frequential attenuation that sounds may suffer
	void FrequentialAttenuation(bool active) const
	{
		generalConfig->Set(3, active);
	}

	// Allows the library to attenuate the sound volume if there are bodies inbetween
	// the listener and the emmiter
	void SoundOclusion(bool active) const
	{
		generalConfig->Set(4, active);
	}

	// Allows the library to calculate the time that the sound lasts to
	// reach the listener and apply it as a delay
	void SoundDelay(bool active) const
	{
		generalConfig->Set(5, active);
	}

	// Allows to debug body collisions. If enabled, "GetCollisionsIterable()" inside NPL class will no longer return null. 
	// Then draw the rectangle inside it. Enabling this might slightly slow the code iteration
	void CollisionsDebugging(bool enable) const { generalConfig->Set(0, enable); }

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

	// Set the global multiplier of the world
	// It scales all the forces applied inside
	void GlobalMultiplier(float multiplier) const
	{
		*globalMultiplier = multiplier;
	}

	// Set the global volume attenuation factor
	// 0 for no attenuation, 1 for real attenuation, >1 for more attenuation
	void GlobalVolumeAttenuationFactor(float factor) const
	{
		*volumeAttenuationFactor = factor;
	}

private:

	float* panRange = nullptr;
	float* panFactor = nullptr;
	float* pitchFactor = nullptr;
	Flag* generalConfig = nullptr;
	Flag* bodiesConfig = nullptr;
	float* pixelsToMeters = nullptr;
	float* ptmRatio = nullptr;
	PhysVec* globalGravity = nullptr;
	PhysVec* globalRestitution = nullptr;
	PhysVec* globalFriction = nullptr;
	unsigned int* physIterations = nullptr;
	float* globalMultiplier = nullptr;
	float* volumeAttenuationFactor = nullptr;

	PhysAction<unsigned int, PhysID>* notifier = nullptr;
};
