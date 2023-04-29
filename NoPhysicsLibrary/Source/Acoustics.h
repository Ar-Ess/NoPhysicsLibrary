#pragma once

#include "PhysRect.h"

class Body;
template<class> class PhysArray;
struct SoundData;
class GasBody;

class Acoustics
{

	friend class NPL;

private:

	Acoustics(PhysArray<Body*>* bodies, PhysArray<SoundData*>* soundDataList, PhysArray<unsigned int*>* gasIndex, float* panRange, float* panFactor);

	void Simulate(Body* b, Body* listener, int soundSize);

	void NoListenerLogic(Body* b);

	void ListenerLogic(Body* b, Body* listener, GasBody* environment);

	GasBody* GetEnvironmentBody(PhysRect body);

	float ComputePanning(float distance, int direction);

	float ComputeVolume(float distance, float spl);

	float ComputeTimeDelay(float distance, GasBody* environment);

private:

	PhysArray<Body*>* bodies = nullptr;
	PhysArray<SoundData*>* soundDataList = nullptr;
	PhysArray<unsigned int*>* gasIndex = nullptr;
	int soundSize = 0;

	const float maxSPL = 120.0f;
	const float maxVolume = 10.0f;

	float* panRange = nullptr;
	float* panFactor = nullptr;

};