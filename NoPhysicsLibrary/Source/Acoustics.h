#pragma once

#include "Ray.h"
#include "PhysRect.h"

class Body;
template<class> class PhysArray;
struct SoundData;
class GasBody;
class LiquidBody;

class Acoustics
{


	struct RaycastAgents
	{
		RaycastAgents()
		{
			this->listener = nullptr;
			this->emitter = nullptr;
		}

		void SetAgents(Body* emitter, Body* listener)
		{
			this->listener = listener;
			this->emitter = emitter;
		}

		Body* listener = nullptr;
		Body* emitter = nullptr;
	};

	struct RayData
	{

		RayData(Body* body, PhysRay ray, RaycastAgents* agents);

		Body* body = nullptr;
		float innerDistance;
		float distance;
		float percentage = 0;
		PhysRay ray = {};
	};

	friend class NPL;

private:

	Acoustics(PhysArray<Body*>* bodies, PhysArray<SoundData*>* soundDataList, PhysArray<unsigned int*>* gasIndex, PhysArray<unsigned int*>* liquidIndex, float* panRange, float* panFactor);

	void Simulate(Body* emitter, Body* listener);

	void NoListenerLogic(Body* emmiter);

	void ListenerLogic(PhysArray<RayData*>* data, const float totalDistance);

	void CalculatePercentages(PhysArray<RayData*>* data, const float totalDistance);

	void CalculatePercentagesVoidSecure(PhysArray<RayData*>* data, const float totalDistance);

	GasBody* GetEnvironmentBody();

	LiquidBody* GetFloodBody();

	float ComputePanning(float distance, int direction);

	float ComputeVolume(float distance, float spl);

	float ComputeTimeDelay(float distance, Body* environment);

	float ComputeVolumeAttenuation(float distance, Body* obstacle);

	float ComputeFrequentialAttenuation(float distance, Body* obstacle, float currentAttenuation);
	
	float ComputePitchShifting(float distance, Body* obstacle);
	
	float ComputeDoppler(float distance, Body* obstacle);

	float ComputeReverb(float distance, Body* obstacle);

	void RayCastBodyList(PhysArray<RayData*>* returnList, PhysRay ray);

private:

	PhysArray<Body*>* bodies = nullptr;
	PhysArray<SoundData*>* soundDataList = nullptr;
	PhysArray<unsigned int*>* gasIndex = nullptr;
	PhysArray<unsigned int*>* liquidIndex = nullptr;
	RaycastAgents* agents = nullptr;

	const float maxSPL = 120.0f;
	const float maxVolume = 10.0f;

	float* panRange = nullptr;
	float* panFactor = nullptr;

};