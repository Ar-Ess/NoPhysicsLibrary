#pragma once
#include "Rect.h"

class Camera
{
public:

	struct FollowMode
	{
		FollowMode(Rect* rect, Point* position) : 
			rect(rect),
			position(position) {}

		void OffsetFollow(Point offset, bool followHorizontal = true, bool followVertical = true);
		void HLimitFollow(float maxLimitLeft, float maxLimitRight, float centerOffset = 0, bool limitLeft = true, bool limitRight = true);
		void VLimitFollow(float maxLimitTop, float maxLimitBottom, float centerOffset = 0, bool limitTop = true, bool limitBottom = true);
		void LimitFollow(Rect limits, float centerOffset = 0 );

	private:
		Rect* rect = nullptr;
		Point* position = nullptr;
	};

public:

	Camera()
	{
		mode = new FollowMode(&rect, &position);
	}

	~Camera()
	{
		delete mode;
	}

	FollowMode* Update(Point position);

	Rect rect = {};

private:

	Point position = {};
	FollowMode* mode = nullptr;

};
