#pragma once

#include "BodyClassEnum.h"
#include "InUnitsEnum.h"

#include "Flag.h"
#include "AcusticData.h"
#include "Define.h"
#include "Rect.h"
#include <vector>

class Body
{
public: // Methods

	virtual ~Body() {}

	// Plays a sound, volume range [0, 120]
	//-TOCHECK: Debug emission point
	void Play(int index, float decibels = 80);

	// Sets the body emission point, where the body sound will emit from.
	// Values out of body bounds will be set to the closest point inside it
	// X increments from left to right. Y increments from top to bottom
	void SetEmissionPoint(Point point, InUnit unit);

	// Sets the body emission point, where the body sound will emit from.
	// Values out of body bounds will be set to the closest point inside it
	void SetEmissionPoint(Align align, Point offset, InUnit unit);

	// Returns the x & y coordinates of the body
	Point GetPosition(InUnit unit) const;

	// Returns the width & height of the body
	Point GetSize(InUnit unit) const;

	// Returns the pointer to the rectangle of the body
	Rect GetRect(InUnit unit) const { return { GetPosition(unit), GetSize(unit) }; }

	// Returns the body class enum
	inline BodyClass GetClass() const { return clas; }

	// Returns the mass of the body
	inline float GetMass() const { return mass; }

	// Returns the body id
	inline intptr_t GetId() const { return id; }

	// Returns wether the body is collidable
	inline bool IsCollidable() const { return properties.Get(0); }

	// Sets the collidable property of the body
	//-TODONE: implement it in the physics engine
	inline void SetCollidable(bool set) { properties.Set(0, set); }

protected: // Methods

	Body(BodyClass clas, Rect rect, float mass, const float* pixelsToMeters);

protected: // Variables

	friend class NPL;

	Point emissionPoint = { 0.0f, 0.0f };
	Rect rect = {};
	BodyClass clas = BodyClass::EMPTY_BODY;
	float mass = 1.0f;
	intptr_t id = 0;
	const float* pixelsToMeters = nullptr;
	std::vector<AcousticData*> acousticDataList;

	// Collidable | 
	Flag properties = {};

};
