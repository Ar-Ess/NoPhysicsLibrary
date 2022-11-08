#pragma once

#include "Body.h"
#include "BodyStateEnum.h"
#include "BodyBackup.h"

class DynamicBody : public Body
{
public: // Methods

	DynamicBody(Rect rect, Point gravityOffset, float mass, Flag* globals, const float* pixelsToMeters);

	~DynamicBody() override;

	// Returns if this body is colliding with any other body in the specified situation
	bool IsColliding(BodyState collision);
	
	// Applies an specific force to this body
	void ApplyForce(float newtonsX, float newtonsY, InUnit unit = InUnit::IN_METERS);
	// Applies an specific force to this body
	void ApplyForce(Point newtons, InUnit unit = InUnit::IN_METERS);

	// Applies an specific force to this body
	void ApplyMomentum(float momentumX, float momentumY, InUnit unit = InUnit::IN_METERS);
	// Applies an specific force to this body
	void ApplyMomentum(Point momentum, InUnit unit = InUnit::IN_METERS);

	// Sets a local gravity to the body
	void SetGravityOffset(Point gravityOffset, InUnit unit = InUnit::IN_METERS);
	// Returns the local gravity vector of the body
	Point GetGravityOffset(InUnit unit = InUnit::IN_METERS) const;

	void ResetForces();

private: // Methods

	void SecondNewton();
	void FirstBuxeda();

	void Backup();

public: // Variables

private: // Variables

	friend class Physics; // Necessary for accessing SecondNewton/FirstBuxeda
	friend class NPL; // Necessary to update states

	Point velocity = {};
	Point gravityOffset = {};
	Point acceleration = {};
	Point coeficientRestitution = { 1.0f,1.0f };
	Point frictionCoeficient = { 0.0f,0.0f };

	std::vector<Force*> forces;
	std::vector<Momentum*> momentums;
	Force totalForces = {};
	Momentum totalMomentum = {};

	// Gas | Ground | Roof | Left | Right | Liquid | Float
	Flag bodyState = {};

	Flag* globals = nullptr;

	BodyBackup backup = {};
};
