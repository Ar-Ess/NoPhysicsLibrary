#pragma once

#include "Body.h"

class StaticBody : public Body
{
public: // Methods

	~StaticBody() override;

	PhysVec RestitutionOffset() const { return restitutionOffset; }

	void RestitutionOffset(PhysVec offset);

	void SetMaterial(Material material) override;

	float YoungModulus() const
	{
		return youngModulus;
	}

private:

	StaticBody(PhysRect rect, float mass, const float* pixelsToMeters);

private:

	friend struct BodyCreation;

	PhysVec restitutionOffset = {0.0f, 0.0f};
	float youngModulus = 0;

};
