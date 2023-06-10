#include "Acoustics.h"
#include "SoundData.h"
#include "GasBody.h"
#include "PhysArray.h"

Acoustics::RayData::RayData(Body* body, PhysRay ray, Acoustics::RaycastAgents agents)
{
	this->body = body;
	this->innerDistance = ray.Distance();
	if (agents.emmiter != nullptr) this->distance = PhysMath::Distance(agents.emmiter->rect.Position(), ray.start);
	else this->distance = innerDistance;

	this->ray = ray;
	this->agents = agents;
}

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

	GasBody* environment = GetEnvironmentBody(PhysRect(float(b->EmissionPoint(InUnit::IN_METERS).x), float(b->EmissionPoint(InUnit::IN_METERS).y), 1.0f, 1.0f));
	if (environment == nullptr) return; // This assures us that the emitter is inside a gas

	PhysRay ray = PhysRay(b->EmissionPoint(InUnit::IN_METERS), listener->ReceptionPoint(InUnit::IN_METERS));
	const float totalDistance = PhysMath::Distance(ray);

	PhysArray<RayData*> data;
	RayCastBodyList(&data, ray, RaycastAgents(listener, b));

	data.Sort([](RayData* a, RayData* b) { return a->distance > b->distance; });

	RayData find = RayData(environment, PhysRay(), RaycastAgents());
	if (!data.Contains(&find))
	{
		data.Add(new RayData(environment, PhysRay(b->EmissionPoint(InUnit::IN_METERS), listener->ReceptionPoint(InUnit::IN_METERS)), RaycastAgents(listener, b)));
		ListenerLogic(&data, totalDistance); // If it's not contained here means that the environment is englobing emissor and listener
	}
	else 
		ListenerLogicVoidSecure(&data, totalDistance); // If it is contained, it is possible to have void inbetween

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

void Acoustics::ListenerLogic(PhysArray<RayData*>* data, const float totalDistance)
{

}

//void Acoustics::ListenerLogic(Body* b, Body* listener, GasBody* environment)
//{
//	for (unsigned int i = 0; i < b->acousticDataList.Size(); ++i)
//	{
//		AcousticData* data = b->acousticDataList[i];
//
//		// Get the distance between Body & Listener
//		float distance = PhysMath::Distance(listener->ReceptionPoint(InUnit::IN_METERS), data->emissionPosition);
//		bool direction = (listener->ReceptionPoint(InUnit::IN_METERS).x - data->emissionPosition.x) < 0;
//
//		float pan = ComputePanning(distance, direction ? 1 : -1);
//
//		float volume = ComputeVolume(distance, data->spl);
//
//		float timeDelay = ComputeTimeDelay(distance, environment);
//
//		soundDataList->Add(new SoundData(data->index, pan, volume, timeDelay));
//	}
//	b->acousticDataList.Clear();
//}

void Acoustics::ListenerLogicVoidSecure(PhysArray<RayData*>* data, const float totalDistance)
{
	float count = 0;
	data->Iterate<float, const float> // Void check
	(
		[](RayData* data, float count, const float totalDistance)
		{
			count += data->innerDistance;
			if (count >= totalDistance) return;
		},
		count,
		totalDistance
	);
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
					ret->Add(new RayData(b, outRay, agents));
					break;
				}

				if (!first || collision) return; // Code ONLY arribes here is first is true and collision is false

				PhysRay outRay;
				if (PhysMath::CheckCollision(agents.listener->Rect(InUnit::IN_METERS), b->Rect(InUnit::IN_METERS))) outRay = { intr[0], agents.listener->ReceptionPoint(InUnit::IN_METERS)}; // Ray of the collision rectangles
				else outRay = { agents.emmiter->EmissionPoint(InUnit::IN_METERS), intr[0]};
				ret->Add(new RayData(b, outRay, agents));

			},
			returnList,
			ray,
			agents
		);
}
