#pragma once

struct Timer
{

	void Start();

	double ReadSecs() const;

private:
	
	Timer();

	double GetSecs() const;

private:

	friend class Sound;

	double startTime = -1;
};
