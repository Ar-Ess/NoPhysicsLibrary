#pragma once

#include "Ray.h"
#include "PhysRect.h"

class Body;
template<class> class PhysArray;
struct SoundData;
class GasBody;

class Acoustics
{

	friend class NPL;

	struct RayData
	{

		RayData(Body* body, PhysRay ray)
		{
			this->body = body;
			this->distance = ray.Distance();
			this->ray = ray;
		}

		Body* body;
		float distance;
		PhysRay ray = {};
	};

	struct RaycastAgents
	{
		RaycastAgents(Body* listener, Body* emmiter)
		{
			this->listener = listener;
			this->emmiter = emmiter;
		}

		Body* listener = nullptr;
		Body* emmiter = nullptr;
	};

private:

	Acoustics(PhysArray<Body*>* bodies, PhysArray<SoundData*>* soundDataList, PhysArray<unsigned int*>* gasIndex, float* panRange, float* panFactor);

	void Simulate(Body* b, Body* listener);

	void NoListenerLogic(Body* b);

	void ListenerLogic(Body* b, Body* listener, GasBody* environment);

	GasBody* GetEnvironmentBody(PhysRect rect);

	float ComputePanning(float distance, int direction);

	float ComputeVolume(float distance, float spl);

	float ComputeTimeDelay(float distance, GasBody* environment);

	void RayCastBodyList(PhysArray<RayData*>* returnList, PhysRay ray, RaycastAgents agents);

private:

	PhysArray<Body*>* bodies = nullptr;
	PhysArray<SoundData*>* soundDataList = nullptr;
	PhysArray<unsigned int*>* gasIndex = nullptr;

	const float maxSPL = 120.0f;
	const float maxVolume = 10.0f;

	float* panRange = nullptr;
	float* panFactor = nullptr;

};