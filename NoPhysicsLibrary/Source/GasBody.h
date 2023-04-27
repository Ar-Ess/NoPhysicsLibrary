#pragma once

#include "Body.h"

class GasBody : public Body
{
public:

	~GasBody() override;
	
	float HeatRatio() const;
	
	float Pressure() const;

	PhysVec DragCoefficient() const
	{
		return dragCoefficient;
	}

	PhysVec LiftCoefficient() const
	{
		return liftCoefficient;
	}

	void SetMaterial(Material material) override;

private:
	
	GasBody(PhysRect rect, float mass, float pressure, PhysVec dragCoefficient, const float* pixelsToMeters);

private:

	friend struct BodyCreation;

	float pressure = 0;
	float heatRatio = 0;
	PhysVec dragCoefficient = {1.0f, 1.0f};
	PhysVec liftCoefficient = {0.0f, 0.0f};
};
