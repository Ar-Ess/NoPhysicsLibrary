#pragma once

#include "Flag.h"

struct LibraryConfig
{
	LibraryConfig(float* panRange, Flag* physicsConfig) : 
		panRange(panRange),
		physicsConfig(physicsConfig)
	{}

	void PanRange(float panRange = 1000) { *this->panRange = panRange; }

	// Allows getting std::vector* of collisions and drawing its rectangle
	// Enabling this might slightly slow the code iteration
	void CollisionsDebugging(bool enable) { this->physicsConfig->Set(0, enable); }

private:

	float* panRange = nullptr;
	Flag* physicsConfig = nullptr;
};
