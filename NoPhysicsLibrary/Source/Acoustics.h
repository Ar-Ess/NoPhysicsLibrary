#pragma once

#include "Ray.h"
#include "PhysRect.h"

class Body;
template<class> class PhysArray;
struct SoundData;
class GasBody;

class Acoustics
{


	struct RaycastAgents
	{
		RaycastAgents()
		{
			this->listener = nullptr;
			this->emmiter = nullptr;
		}

		RaycastAgents(Body* listener, Body* emmiter)
		{
			this->listener = listener;
			this->emmiter = emmiter;
		}

		Body* listener = nullptr;
		Body* emmiter = nullptr;
	};

	struct RayData
	{

		RayData(Body* body, PhysRay ray, RaycastAgents agents);

		Body* body = nullptr;
		float innerDistance;
		float distance;
		PhysRay ray = {};
		RaycastAgents agents;
	};

	friend class NPL;

private:

	Acoustics(PhysArray<Body*>* bodies, PhysArray<SoundData*>* soundDataList, PhysArray<unsigned int*>* gasIndex, float* panRange, float* panFactor);

	void Simulate(Body* b, Body* listener);

	void NoListenerLogic(Body* b);

	void ListenerLogic(PhysArray<RayData*>* data, const float totalDistance);

	void ListenerLogicVoidSecure(PhysArray<RayData*>* data, const float totalDistance);

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