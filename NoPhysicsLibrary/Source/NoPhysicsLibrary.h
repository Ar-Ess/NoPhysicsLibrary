#pragma once
#include "BodyCreationAux.h"
#include "Body.h"

namespace NPL
{
	void Init();

	BodyCreation CreateBody(Rect rectangle, float mass);

	bool Update(float dt);
}