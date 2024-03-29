#pragma once

#include "Body.h"

class LiquidBody : public Body
{
public:

	~LiquidBody() override;

	float Volume(InUnit unit) const;

	float Density(InUnit unit) const;
	void Density(float density, InUnit unit);

	float Buoyancy() const;
	void Buoyancy(float buoyancy);
	
	float Viscosity() const;
	
	float BulkModulus() const;

	PhysVec LiftCoefficient() const
	{
		return liftCoefficient;
	}

	PhysVec DragCoefficient() const
	{
		return dragCoefficient;
	}

	void SetMaterial(Material material) override;

private:

	LiquidBody(PhysRect rect, float mass, float buoyancy, const float* pixelsToMeters);

private:

	friend struct BodyCreation;

	float buoyancy = 0.0f;
	float viscosity = 0.0f;
	float bulkModulus = 0.0f;
	PhysVec liftCoefficient = {0.1f, 0.0f};
	PhysVec dragCoefficient = {1.0f, 0.0f};
};
