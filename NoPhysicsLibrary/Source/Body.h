#pragma once

#include "BodyClassEnum.h"
#include "InUnitsEnum.h"

#include "Flag.h"
#include "AcusticData.h"
#include "Define.h"
#include "PhysMath.h"
#include "PhysArray.h"
#include "PhysID.h"
#include "NoMaterial.h"

class Body
{
public: // Methods

	enum BodyPropierty
	{
		BP_COLLIDABLE,
		BP_PHYSICS_UPDATE,
		BP_ACOUSTICS_UPDATE
	};

	virtual ~Body() {}

	// Plays a sound, volume range [0, 120]
	//-TOCHECK: Debug emission point
	void Play(unsigned int index, float decibels = 80);

	// Sets the emission point of the body. It is an offset from the center of the body.
	// The emission point is from where the body will emmit sound.
	void EmissionPoint(PhysVec offset, InUnit unit = InUnit::IN_PIXELS);
	// Returns the emission point of the body. It is an offset from the center of the body.
	// The emission point is from where the body will emmit sound.
	PhysVec EmissionPoint(InUnit unit) const;

	// Sets the reception point of the body. It is an offset from the center of the body.
	// The reception point is from where the body listener will receive the sound.
	void ReceptionPoint(PhysVec offset, InUnit unit = InUnit::IN_PIXELS);
	// Returns the reception point of the body. It is an offset from the center of the body.
	// The reception point is from where the body listener will receive the sound.
	PhysVec ReceptionPoint(InUnit unit) const;

	// Returns the top-left x & y coordinates of the body
	PhysVec Position(InUnit unit) const;
	// Sets the top-left x & y coordinates of the body
	void Position(float x, float y, InUnit unit);
	// Sets the top-left x & y coordinates of the body
	void Position(PhysVec position, InUnit unit);

	// Returns the width & height of the body
	PhysVec Size(InUnit unit) const;

	// Returns the pointer to the rectangle of the body
	PhysRect Rect(InUnit unit) const;

	// Returns the body class enum
	BodyClass Class() const { return clas; }

	// Returns the mass of the body
	float Mass() const { return mass; }
	// Set the material's mass
	void Mass(float mass) { mass <= 0 ? this->mass = 0.1f : this->mass = mass; }

	// Returns the material of the body
	NoMaterial Material() const { return material; }
	// Sets the material of the body
	void Material(NoMaterial material) { this->material = material; }

	// Returns the body id
	PhysID Id() const { return *id; }

	// Returns wether the body is collidable
	bool IsCollidable() const { return properties.Get(BP_COLLIDABLE); }
	// Sets the collidable property of the body
	void IsCollidable(bool set) { properties.Set(BP_COLLIDABLE, set); }
	
	// Returns wether the physics updatability property of the body is setted or not
	bool HasPhysicsUpdatability() const { return properties.Get(BP_PHYSICS_UPDATE); }
	// Sets the physics updatability property of the body
	void HasPhysicsUpdatability(bool set) { properties.Set(BP_PHYSICS_UPDATE, set); }
	
	// Returns wether the acoustics updatability property of the body is setted or not
	bool HasAcousticsUpdatability() const { return properties.Get(BP_ACOUSTICS_UPDATE); }
	// Sets the acoustic updatability property of the body
	void HasAcousticsUpdatability(bool set) { properties.Set(BP_ACOUSTICS_UPDATE, set); }

	bool operator==(Body* b)
	{
		return *id == *b->id;
	}

protected: // Methods

	Body(BodyClass clas, PhysRect rect, float mass, const float* pixelsToMeters);

	const float Conversion(InUnit unit, bool isSetter, int times = 1) const
	{
		if (unit == InUnit::IN_METERS) return 1.0f;

		return PhysMath::Pow(isSetter ? *pixelsToMeters : 1 / *pixelsToMeters, (float)times);
	}

private: // Methods

	PhysVec GetEmissionPointOffset(InUnit unit)
	{
		return (emissionPoint * Conversion(unit, false));
	}

protected: // Variables

	friend class NPL;

	PhysRect rect = {};
	BodyClass clas = BodyClass::EMPTY_BODY;
	float mass = 1.0f;
	PhysID* id = nullptr;
	// Units: m/pxl
	const float* pixelsToMeters = nullptr;
	PhysArray<AcousticData*> acousticDataList;
	NoMaterial material = {};

private:

	PhysVec emissionPoint = { 0.0f, 0.0f };

	PhysVec receptionPoint = { 0.0f, 0.0f };

	// Collidable(0) | UpdatePhysics(1) | UpdateAcoustics(2)
	Flag properties = {};

};
