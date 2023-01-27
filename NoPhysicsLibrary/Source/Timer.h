#pragma once

struct Timer
{
	Timer();

	void Start();

	double ReadSecs() const;

private:

	double GetSecs() const;

private:

	double startTime = -1;
};
