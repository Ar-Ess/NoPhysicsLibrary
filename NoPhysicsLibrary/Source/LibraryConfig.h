#pragma once

struct LibraryConfig
{
	LibraryConfig(float* panRange) : 
		panRange(panRange)
	{}

	void PanRange(float panRange = 1000) { *this->panRange = panRange; }

private:

	float* panRange = nullptr;
};
