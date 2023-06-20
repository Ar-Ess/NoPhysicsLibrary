#pragma once

#include "Ray.h"
#include "PhysRect.h"
#include "Define.h"

class Body;
template<class> class PhysArray;
struct SoundData;
class GasBody;
class LiquidBody;
class Flag;

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

		bool operator==(RayData data) const;

	};

	friend class NoPhysicsLibrary;

private:

	Acoustics(PhysArray<Body*>* bodies, PhysArray<SoundData*>* soundDataList, PhysArray<unsigned int*>* gasIndex, PhysArray<unsigned int*>* liquidIndex, const float* panRange, const float* panFactor, const float* pitchVariationFactor, const Flag* generalConfig, const float* globalVolumeAttFactor);

	void Simulate(Body* emitter, Body* listener);

	void NoListenerLogic(Body* emmiter);

	void ListenerLogic(PhysArray<RayData*>* data, const float totalDistance);

	bool VoidSecurityLogic(PhysArray<RayData*>* data, GasBody* environment, LiquidBody* flood, const float totalDistance);

	GasBody* GetEnvironmentBody();

	LiquidBody* GetFloodBody();

	float ComputePanning(float distance, int direction);

	float ComputeVolume(float distance, float spl);

	float ComputeTimeDelay(float distance, Body* environment, float& outVelocity);

	float ComputeVolumeAttenuation(float distance, Body* obstacle);

	void ComputeFrequentialAttenuation(float innerDistance, float distance, float velocity, Body* obstacle, float& outCutoff, float& outResonance);

	void ComputePitchShifting(float velocity, const float percent, float& outPitch);

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

	const float* panRange = nullptr;
	const float* panFactor = nullptr;
	const float* pitchVariationFactor = nullptr;
	const Flag* generalConfig = nullptr;
	const float* globalVolumeAttFactor = nullptr;

};