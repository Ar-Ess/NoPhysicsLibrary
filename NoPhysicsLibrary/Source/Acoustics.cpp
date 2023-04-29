#include "Acoustics.h"
#include "SoundData.h"
#include "GasBody.h"

Acoustics::Acoustics(PhysArray<Body*>* bodies, PhysArray<SoundData*>* soundDataList, PhysArray<unsigned int*>* gasIndex, float* panRange, float* panFactor)
{
	this->bodies = bodies;
	this->soundDataList = soundDataList;
	this->panRange = panRange;
	this->panFactor = panFactor;
	this->gasIndex = gasIndex;
}

void Acoustics::Simulate(Body* b, Body* listener)
{
	if (!listener) return NoListenerLogic(b);

	GasBody* environment = GetEnvironmentBody(b->Rect(InUnit::IN_METERS));

	if (!environment) return b->acousticDataList.Clear();

	ListenerLogic(b, listener, environment);
}

void Acoustics::NoListenerLogic(Body* b)
{
	b->acousticDataList.Iterate<int, PhysArray<SoundData*>*, const float>
	(
		[](AcousticData* data, PhysArray<SoundData*>* soundDataList, const float maxSPL)
		{
			soundDataList->Add(new SoundData(data->index, 0, data->spl / maxSPL, 0));
		},
		soundDataList, maxSPL
	);

	b->acousticDataList.Clear();
}

void Acoustics::ListenerLogic(Body* b, Body* listener, GasBody* environment)
{
	// If listener emit sound
	if (b->Id() == listener->id) return NoListenerLogic(b);
	if (b->acousticDataList.Empty()) return;

	for (unsigned int i = 0; i < b->acousticDataList.Size(); ++i)
	{
		AcousticData* data = b->acousticDataList[i];

		// Get the distance between Body & Listener
		float distance = PhysMath::Distance(listener->ReceptionPoint(InUnit::IN_METERS), data->emissionPosition);
		bool direction = (listener->ReceptionPoint(InUnit::IN_METERS).x - data->emissionPosition.x) < 0;

		float pan = ComputePanning(distance, direction ? 1 : -1);

		float volume = ComputeVolume(distance, data->spl);

		float timeDelay = ComputeTimeDelay(distance, environment);

		soundDataList->Add(new SoundData(data->index, pan, volume, timeDelay));
	}
	b->acousticDataList.Clear();
}

GasBody* Acoustics::GetEnvironmentBody(PhysRect body)
{
	for (unsigned int i = 0; i < gasIndex->Size(); ++i)
	{
		Body* b = bodies->At(*(*gasIndex)[i]);
		if (PhysMath::CheckCollision(body, b->Rect(InUnit::IN_METERS)))
			return (GasBody*)b;
	}

	return nullptr;
}

float Acoustics::ComputePanning(float distance, int direction)
{
	// Narrow down distance over Range for panning operations
	PhysMath::Clamp(distance, 0, *panRange);

	// Compute pan (normalized between [ 1, -1])
	return PhysMath::Pow(distance / *panRange, *panFactor) * direction;
}

float Acoustics::ComputeVolume(float distance, float spl)
{
	// Compute the sound attenuation over distance
	// Final SPL = Initial SPL - 20*Log(distance / 1)
	float fSPL = spl - 20 * PhysMath::Log(distance);

	// Transform volume from db to linear [ 0, 1])
	return PhysMath::LogToLinear(fSPL, maxSPL) / maxVolume;
}

float Acoustics::ComputeTimeDelay(float distance, GasBody* environment)
{
	// Calculate delay time
	// 1. Vel = sqrt( lambda * Pa / ro )
	// 2. Time = distance / vel
	float timeDelay = 0;
	float vel = PhysMath::Sqrt(environment->HeatRatio() * environment->Pressure() / environment->Density(InUnit::IN_METERS));
	return distance / vel;
}
