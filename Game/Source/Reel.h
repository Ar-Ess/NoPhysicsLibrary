#ifndef __REEL_H__
#define __REEL_H__

#include <vector>
#include "Rect.h"
#include "Point.h"
#include <assert.h>
typedef unsigned short int suint;

class Reel
{
public:

	// Creates a reel from a frame given the rows & cols
	//  - Use Rect's X & Y as an offset
	//  - Use Rect's W & H to give the constant width & height of each frame
	Reel(Rect frameRect, suint rows, suint columns)
	{
		// A reel can't be initialized if there are not rows/columns
		assert(rows > 0 && columns > 0);

		for (suint i = 0; i < rows; ++i)
		{
			for (suint a = 0; a < columns; ++a) frames.push_back(Rect{ frameRect.GetPosition().Apply({frameRect.w * i, frameRect.h * a}), frameRect.GetDimensions() });
		}
	}

	inline void PushBack(Rect rect)
	{
		frames.push_back(rect);
	}

	void Reset()
	{
		currentFrame = 0.0f;
		loopCount = 0;
		pingpongDirection = true;
	}

	inline bool HasFinished()
	{
		return currentFrame < 1.0f;
	}

	void Update(float dt)
	{
		currentFrame += speed * dt;
		suint totalFrames = GetTotalFrames();

		if (currentFrame >= totalFrames)
		{
			currentFrame = (loop || pingpong) ? 0.0f : totalFrames - 1;
			++loopCount;

			if (pingpong)
				pingpongDirection = !pingpongDirection;
		}
	}

public: // Getters

	inline const suint GetTotalFrames() const
	{
		return (const suint)frames.size();
	}

	inline Rect GetLastFrame() const
	{
		return *frames.end();
	}

	Rect GetCurrentFrame() const
	{
		int actualFrame = (int)currentFrame;

		if (!pingpongDirection)
			actualFrame = GetTotalFrames() - currentFrame;

		return frames[actualFrame];
	}

public:

	float speed = 1.0f;
	bool loop = true;
	bool pingpong = false;

private:
	std::vector<Rect> frames;
	float currentFrame = 0.0f;
	int loopCount = 0;
	bool pingpongDirection = true; // True = R, False = L
};

#endif
