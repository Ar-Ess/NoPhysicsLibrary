#pragma once

#include "BodyClassEnum.h"
#include "BodyBackup.h"
#include "Flag.h"

class Body
{
public: // Methods

	Body(BodyClass clas = BodyClass::EMPTY_BODY, Rect rect = { 0.0f, 0.0f, 1.0f, 1.0f }, float mass = 1.0f);

	void SolveCollision(Body& body, int dir);

	void DeClipper(Body& body, int dir);

public: //Getters

	// Returns the x & y coordinates of the body
	inline Point GetPosition() const { return rect.GetPosition(); }

	// Returns the width & height of the body
	inline Point GetSize() const { return rect.GetSize(); }

	// Returns the pointer to the rectangle of the body
	inline Rect GetRect() const { return rect; }

	// Returns the body class enum
	inline BodyClass GetBodyClass() const { return clas; }

	// Returns the mass of the body
	inline float GetMass() const { return mass; }

	// Returns wether the body is collidable
	inline bool IsCollidable() { return properties.Get(0); }

public: // Setters

	// Sets the collidable property of the body
	inline void SetCollidable(bool set) { properties.Set(set, 1); }

protected:
	friend class Physics;
	Rect rect = {};
	BodyClass clas = BodyClass::EMPTY_BODY;
	float mass = 1.0f;

	// Collidable | 
	Flag properties = {};
};
