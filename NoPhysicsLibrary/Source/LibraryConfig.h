#pragma once

#include "Flag.h"
#include "MathUtils.h"

struct LibraryConfig
{
	LibraryConfig(float* panRange, Flag* physicsConfig, float* pixelsToMeters) :
		panRange(panRange),
		physicsConfig(physicsConfig),
		pixelsToMeters(pixelsToMeters)
	{}

	// Set which distance (in meters) the audio will sound mono in one of the two channels (Left / Right)
	// Remember to configure PixelsToMeters before configuring the panning range
	void PanRange(float panRange, InUnit unit) 
	{ 
		if (unit == InUnit::IN_PIXELS) panRange *= *pixelsToMeters;
		*this->panRange = panRange > 0 ? panRange : 0; 
	}

	// Allows to debug body collisions. If enabled, "GetCollisionsIterable()" will no longer return null. 
	// Then draw the rectangle inside it. Enabling this might slightly slow the code iteration
	void CollisionsDebugging(bool enable) { this->physicsConfig->Set(0, enable); }

private:

	float* panRange = nullptr;
	Flag* physicsConfig = nullptr;
	float* pixelsToMeters = nullptr;
};
