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
	void PanRange(float panRange) { *this->panRange = panRange > 0 ? panRange : 0; }

	// Allows to debug body collisions. If enabled, "GetCollisionsIterable()" will no longer return null. 
	// Then draw the rectangle inside it. Enabling this might slightly slow the code iteration
	void CollisionsDebugging(bool enable) { this->physicsConfig->Set(0, enable); }

	// Set how many pixels constitute a meter in your world
	// Invalid values setted directly to 1
	void PixelsToMeters(float ratio) { *this->pixelsToMeters = ratio > 0 ? 1 / ratio : 1; }

private:

	float* panRange = nullptr;
	Flag* physicsConfig = nullptr;
	float* pixelsToMeters = nullptr;
};
