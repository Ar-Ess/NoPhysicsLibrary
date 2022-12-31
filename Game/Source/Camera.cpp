#include "Camera.h"

Camera::FollowMode* Camera::Update(Point position)
{
    this->position = position;
    return mode;
}

void Camera::FollowMode::OffsetFollow(Point offset, bool followHorizontal, bool followVertical)
{
	if (!followVertical && !followHorizontal) return;

	if (followHorizontal) rect->x = position->x - offset.x;
	if (followVertical) rect->y = position->y - offset.y;
}

void Camera::FollowMode::HLimitFollow(float maxLimitLeft, float maxLimitRight, float centerOffset, bool limitLeft, bool limitRight)
{
	float offset = (rect->w / 2) + centerOffset;
	if (!limitLeft && !limitRight) return OffsetFollow(offset, true, false);
	rect->x = position->x - offset;

	//if (position->x >= offset) rect->x = position->x - offset;

	if (rect->x <  maxLimitLeft) rect->x =  maxLimitLeft;
	if (rect->x > maxLimitRight) rect->x = maxLimitRight;

}

void Camera::FollowMode::VLimitFollow(float maxLimitTop, float maxLimitBottom, float centerOffset, bool limitTop, bool limitBottom)
{
	float offset = (rect->h / 2) + centerOffset;
	if (!limitTop && !limitBottom) return OffsetFollow(offset, false, true);
	rect->y = position->y - offset;

	//if (position->y >= offset) rect->y = position->y - offset;

	if (limitTop && rect->y < maxLimitTop) rect->y = maxLimitTop;
	if (limitBottom && rect->y > maxLimitBottom) rect->y = maxLimitBottom;
}

void Camera::FollowMode::LimitFollow(Rect limits, float centerOffset)
{
	HLimitFollow(limits.x, limits.w, centerOffset);
	VLimitFollow(limits.y, limits.h, centerOffset);
}
