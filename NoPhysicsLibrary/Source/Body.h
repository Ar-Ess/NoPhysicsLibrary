#pragma once

#include "BodyClassEnum.h"
#include "BodyBackup.h"
#include "Flag.h"
#include "Audio.h"

class Body
{
public: // Methods

	Body(BodyClass clas = BodyClass::EMPTY_BODY, Rect rect = { 0.0f, 0.0f, 1.0f, 1.0f }, float mass = 1.0f);

	virtual ~Body() {}

	// Plays a sound
	void Play(int index, float decibels = 0) { soundList.emplace_back(new SoundData(index, rect.GetCentricPosition(), decibels)); }

	// Returns the x & y coordinates of the body
	inline Point GetPosition() const { return rect.GetPosition(); }

	// Returns the width & height of the body
	inline Point GetSize() const { return rect.GetSize(); }

	// Returns the pointer to the rectangle of the body
	inline Rect GetRect() const { return rect; }

	// Returns the body class enum
	inline BodyClass GetClass() const { return clas; }

	// Returns the mass of the body
	inline float GetMass() const { return mass; }

	// Returns the body id
	inline intptr_t GetId() const { return id; }

	// Returns wether the body is collidable
	inline bool IsCollidable() { return properties.Get(0); }

	// Sets the collidable property of the body
	inline void SetCollidable(bool set) { properties.Set(set, 1); }

protected: // Functions

	void SolveCollision(Body& body, int dir);

	void DeClipper(Body& body, int dir);

protected: // Variables

	friend class NPL;

	Rect rect = {};
	BodyClass clas = BodyClass::EMPTY_BODY;
	float mass = 1.0f;
	intptr_t id = 0;

	std::vector<SoundData*> soundList;

	// Collidable | 
	Flag properties = {};

};
