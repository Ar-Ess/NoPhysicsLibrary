#pragma once

#include "Body.h"

class StaticBody : public Body
{
public: // Methods

	StaticBody(PhysRect rect, float mass, const float* pixelsToMeters);

	~StaticBody() override;

	PhysVec RestitutionOffset() const { return restitutionOffset; }

	void RestitutionOffset(PhysVec offset);

private:

	PhysVec restitutionOffset = {0.0f, 0.0f};

};
