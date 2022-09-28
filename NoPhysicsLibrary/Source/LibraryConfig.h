#pragma once

#include "Flag.h"
#include "MathUtils.h"

struct LibraryConfig
{
	LibraryConfig(float* panRange, Flag* physicsConfig) : 
		panRange(panRange),
		physicsConfig(physicsConfig)
	{}

	// Panning Range allows to determine which distance the audio will sound mono in one of the two channels (Left / Right)
	void PanRange(float panRange = 1000) { *this->panRange = panRange > 10 ? panRange : 10; }

	// Allows to debug body collisions. If enabled, "GetCollisionsIterable()" will no longer return null. 
	// Then draw the rectangle inside it. Enabling this might slightly slow the code iteration
	void CollisionsDebugging(bool enable) { this->physicsConfig->Set(0, enable); }

private:

	float* panRange = nullptr;
	Flag* physicsConfig = nullptr;
};
