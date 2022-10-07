#pragma once

struct Timer
{
	Timer();

	void Start();

	int ReadSecs() const;

private:

	int GetSecs() const;

private:

	int startTime = -1;
};
