#include "Acoustics.h"
#include "SoundData.h"
#include "GasBody.h"
#include "PhysArray.h"

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
	if (!listener || listener->id == b->id) return NoListenerLogic(b);

	GasBody* environment = GetEnvironmentBody(PhysRect(float(b->emissionPoint.x), float(b->emissionPoint.y), 1.0f, 1.0f));
	if (environment == nullptr) return;

	PhysRay ray = PhysRay(b->EmissionPoint(InUnit::IN_METERS), listener->ReceptionPoint(InUnit::IN_METERS));
	float distance = PhysMath::Distance(ray);

	PhysArray<RayData*> bodyList;
	RayCastBodyList(&bodyList, ray, RaycastAgents(listener, b));

	for (unsigned int i = 0; i < bodies->Size(); ++i)
	{
		Body* a = (*bodies)[i];
		if (a == listener || a == b) continue;

		PhysMath::RayCast(ray, a->Rect(InUnit::IN_METERS));
	}
}

void Acoustics::NoListenerLogic(Body* b)
{
	b->acousticDataList.Iterate<PhysArray<SoundData*>*, const float>
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

GasBody* Acoustics::GetEnvironmentBody(PhysRect rect)
{
	for (unsigned int i = 0; i < gasIndex->Size(); ++i)
	{
		Body* b = bodies->At(*(*gasIndex)[i]);
		if (PhysMath::CheckCollision(rect, b->Rect(InUnit::IN_METERS)))
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

void Acoustics::RayCastBodyList(PhysArray<RayData*>* returnList, PhysRay ray, RaycastAgents agents)
{
	bodies->Iterate<PhysArray<RayData*>*, PhysRay, RaycastAgents>
		(
			[](Body* b, PhysArray<RayData*>* ret, PhysRay ray, RaycastAgents agents)
			{
				if (b == agents.listener || b == agents.emmiter) return;

				PhysRect rect = b->rect;
				PhysRay rectPlanes[4] =
				{
					{ rect.Position(), b->rect.Position() + PhysVec(rect.w, 0) },
					{ rect.Position() + PhysVec(rect.w, 0), rect.Position() + rect.Size() },
					{ rect.Position() + rect.Size(), rect.Position() + PhysVec(0, rect.h) },
					{ rect.Position() + PhysVec(0, rect.h), rect.Position() }
				};

				PhysVec intr[2] = {};
				bool first = false;
				bool collision = false;

				for (unsigned short int i = 0; i < 4; ++i)
				{
					if (!PhysMath::RayCast_Internal(ray, rectPlanes[i], intr[first])) continue;

					if (!first)
					{
						first = true;
						continue;
					}

					collision = true;
					PhysRay outRay = { intr[0], intr[1]};
					ret->Add(new RayData(b, outRay));
					break;
				}

				if (!first || intr[0].IsZero()) return; // Code ONLY arribes here is first is true and collision is false

				PhysRay outRay = { agents.emmiter->EmissionPoint(InUnit::IN_METERS), intr[0] };
				ret->Add(new RayData(b, outRay));

			},
			returnList,
			ray,
			agents
		);
}
