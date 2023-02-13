#pragma once

#include "BodyClassEnum.h"
#include "InUnitsEnum.h"

#include "Flag.h"
#include "AcusticData.h"
#include "Define.h"
#include "PhysRect.h"
#include <vector>

class Body
{
public: // Methods

	virtual ~Body() {}

	// Plays a sound, volume range [0, 120]
	//-TOCHECK: Debug emission point
	void Play(int index, float decibels = 80);

	// Sets the emission point of the body. Offsetting from the center of the body.
	// The emission point is from where the body will emmit sound.
	void EmissionPoint(PhysVec offset, InUnit offsetUnit = InUnit::IN_PIXELS);
	// Returns the emission point of the body
	// The emission point is from where the body will emmit sound.
	PhysVec EmissionPoint(InUnit unit) const;

	// Returns the top-left x & y coordinates of the body
	PhysVec Position(InUnit unit) const;
	// Sets the top-left x & y coordinates of the body
	PhysVec Position(float x, float y, InUnit unit) const;
	// Sets the top-left x & y coordinates of the body
	PhysVec Position(float x, float y, InUnit unit) const;

	// Returns the width & height of the body
	PhysVec Size(InUnit unit) const;

	// Returns the pointer to the rectangle of the body
	PhysRect Rect(InUnit unit) const;

	
	// Returns the body class enum
	BodyClass GetClass() const { return clas; }

	// Returns the mass of the body
	float GetMass() const { return mass; }

	// Returns the body id
	intptr_t GetId() const { return id; }

	//  Set the object's mass
	void SetMass(float mass) { mass <= 0 ? this->mass = 0.1f : this->mass = mass; }

	// Returns wether the body is collidable
	bool IsCollidable() const { return properties.Get(0); }

	// Sets the collidable property of the body
	void SetCollidability(bool set) { properties.Set(0, set); }
	
	// Sets the physics updatability property of the body
	void SetPhysicsUpdatability(bool set) { properties.Set(1, set); }
	
	// Sets the acoustic updatability property of the body
	void SetAcousticsUpdatability(bool set) { properties.Set(2, set); }

protected: // Methods

	Body(BodyClass clas, PhysRect rect, float mass, const float* pixelsToMeters);

	const float Conversion(InUnit unit, bool isSetter, int times = 1) const
	{
		if (unit == InUnit::IN_METERS) return 1.0f;

		return PhysMath::Pow(isSetter ? *pixelsToMeters : 1 / *pixelsToMeters, times);
	}

private: // Methods

	PhysVec GetEmissionPointOffset(InUnit unit) const
	{
		float conversion = 1.0f;
		if (unit == InUnit::IN_PIXELS) conversion /= *pixelsToMeters;
		return emissionPoint.Multiply(conversion);
	}

protected: // Variables

	friend class NPL;

	PhysVec emissionPoint = { 0.0f, 0.0f };
	PhysRect rect = {};
	BodyClass clas = BodyClass::EMPTY_BODY;
	float mass = 1.0f;
	intptr_t id = 0;
	const float* pixelsToMeters = nullptr;
	std::vector<AcousticData*> acousticDataList;

	// Collidable | UpdatePhysics | UpdateAcoustics
	Flag properties = {};

};
