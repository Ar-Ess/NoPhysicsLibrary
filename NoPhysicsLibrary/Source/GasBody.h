#pragma once

#include "Body.h"

class GasBody : public Body
{
public:

	GasBody(PhysRect rect, float mass, float heatRatio, float pressure, PhysVec dragCoefficient, const float* pixelsToMeters);

	~GasBody() override;

	float GetVolume(InUnit unit);

	float GetDensity(InUnit unit);

	float GetHeatRatio() const;

	float GetPressure() const;

	void SetDensity(float density, InUnit unit);

	void SetHeatRatio(float heatRatio);

	PhysVec DragCoefficient() const
	{
		return dragCoefficient;
	}

	PhysVec LiftCoefficient() const
	{
		return liftCoefficient;
	}

private:

	float heatRatio = 0;
	float pressure = 0;
	PhysVec dragCoefficient = {1.0f, 1.0f};
	PhysVec liftCoefficient = {0.0f, 0.0f};
};
