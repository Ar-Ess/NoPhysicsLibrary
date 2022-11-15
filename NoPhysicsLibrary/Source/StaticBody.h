#pragma once

#include "Body.h"


class StaticBody : public Body
{
public: // Methods

	StaticBody(Rect rect, float mass, const float* pixelsToMeters);

	~StaticBody() override;

	Point GetRestitutionOffset() const { return restitutionOffset; }

	void SetRestitutionOffset(Point offset);

private:

	Point restitutionOffset = {0.0f, 0.0f};

};
