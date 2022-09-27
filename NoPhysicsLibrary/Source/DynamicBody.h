#pragma once

#include "Body.h"
#include "CollideBool.h"
#include "BodyBackup.h"

class DynamicBody : public Body
{
public: // Methods

	DynamicBody(Rect rect, Point velocity, Point gravityOffset, float mass, Flag* globals);

	~DynamicBody() override;

	// Returns if this body is colliding with any other body in the specified situation
	bool IsColliding(CollideBool collision);
	// Returns if this body is colliding with any other body in any possible way
	bool IsColliding();
	
	// Applies an specific force to this body
	void ApplyForce(float newtonsX, float newtonsY);
	// Applies an specific force to this body
	void ApplyForce(Point newtons);

	// Applies an specific force to this body
	void ApplyMomentum(float momentumX, float momentumY);
	// Applies an specific force to this body
	void ApplyMomentun(Point momentum);

	// Sets a local gravity to the body
	void SetGravityOffset(Point gravityOffset);
	// Returns the local gravity vector of the body
	Point GetGravityOffset() const;

private: // Methods

	void SecondNewton();
	void FirstBuxeda();

	void Backup();

public: // Variables

private: // Variables

	friend class Physics; // Necessary for accessing SecondNewton/FirstBuxeda

	Point velocity = {};
	Point gravityOffset = {};
	Point acceleration = {};
	Point coeficientRestitution = { 1.0f,1.0f };
	Point frictionCoeficient = { 0.0f,0.0f };

	std::vector<Force*> forces;
	std::vector<Momentum*> momentums;
	Force totalForces = {};
	Momentum totalMomentum = {};

	// Air | Ground | Roof | Left | Right | Water
	Flag collisionFlags = {};

	Flag* globals = nullptr;

	BodyBackup backup = {};
};
