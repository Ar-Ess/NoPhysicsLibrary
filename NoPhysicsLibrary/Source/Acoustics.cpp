#include "Acoustics.h"
#include "SoundData.h"
#include "GasBody.h"
#include "DynamicBody.h"
#include "StaticBody.h"
#include "LiquidBody.h"
#include "PhysArray.h"

Acoustics::RayData::RayData(Body* body, PhysRay ray, Acoustics::RaycastAgents* agents)
{
	this->body = body;
	this->innerDistance = ray.Distance();
	if (agents != nullptr) this->distance = PhysMath::Distance(agents->emitter->rect.Position(), ray.start);
	else this->distance = innerDistance;

	this->ray = ray;
}

Acoustics::Acoustics(PhysArray<Body*>* bodies, PhysArray<SoundData*>* soundDataList, PhysArray<unsigned int*>* gasIndex, float* panRange, float* panFactor)
{
	this->bodies = bodies;
	this->soundDataList = soundDataList;
	this->panRange = panRange;
	this->panFactor = panFactor;
	this->gasIndex = gasIndex;
	this->agents = new RaycastAgents();
}

void Acoustics::Simulate(Body* emitter, Body* listener)
{
	if (!listener || listener->id == emitter->id) return NoListenerLogic(emitter);

	agents->SetAgents(emitter, listener);

	GasBody* environment = GetEnvironmentBody(PhysRect(float(emitter->EmissionPoint(InUnit::IN_METERS).x), float(emitter->EmissionPoint(InUnit::IN_METERS).y), 1.0f, 1.0f));
	if (environment == nullptr) return; // This assures us that the emitter is inside a gas

	PhysRay ray = PhysRay(emitter->EmissionPoint(InUnit::IN_METERS), listener->ReceptionPoint(InUnit::IN_METERS));
	const float totalDistance = PhysMath::Distance(ray);

	PhysArray<RayData*> data;
	RayCastBodyList(&data, ray);

	data.Sort([](RayData* a, RayData* b) { return a->distance > b->distance; });

	RayData find = RayData(environment, PhysRay(), nullptr);
	if (!data.Contains(&find))
	{
		data.Add(new RayData(environment, PhysRay(emitter->EmissionPoint(InUnit::IN_METERS), listener->ReceptionPoint(InUnit::IN_METERS)), nullptr));
		CalculatePercentages(&data, totalDistance); // If it's not contained here means that the environment is englobing emissor and listener
	}
	else 
		CalculatePercentagesVoidSecure(&data, totalDistance); // If it is contained, it is possible to have void inbetween

	ListenerLogic(&data, totalDistance);

}

void Acoustics::NoListenerLogic(Body* emmiter)
{
	emmiter->acousticDataList.Iterate<PhysArray<SoundData*>*, const float>
	(
		[](AcousticData* data, PhysArray<SoundData*>* soundDataList, const float maxSPL)
		{
			soundDataList->Add(new SoundData(data->index, 0, data->spl / maxSPL, 0));
		},
		soundDataList, maxSPL
	);

	emmiter->acousticDataList.Clear();
}

void Acoustics::ListenerLogic(PhysArray<RayData*>* data, const float totalDistance)
{
	for (unsigned int i = 0; i < agents->emitter->acousticDataList.Size(); ++i)
	{
		AcousticData* aData = agents->emitter->acousticDataList[i];

		// Get the distance between Body & Listener
		bool direction = (agents->listener->ReceptionPoint(InUnit::IN_METERS).x - agents->emitter->EmissionPoint(InUnit::IN_METERS).x) < 0;

		float pan = ComputePanning(totalDistance, direction ? 1 : -1);

		float volume = ComputeVolume(totalDistance, aData->spl);

		if (volume < 0.01) continue;

		float timeDelay = 0;
		bool noVolume = false;

		for (unsigned int i = 0; i < data->Size(); ++i)
		{
			RayData* rD = data->At(i);

			volume -= ComputeVolumeAttenuation(rD->innerDistance, rD->body);
			noVolume = volume < 0.01f;
			if (noVolume) break;

			timeDelay += ComputeTimeDelay(totalDistance, rD->body) * rD->percentage;

			float freq = ComputeFrequentialAttenuation(totalDistance, rD->body) * rD->percentage;

			float pitch = ComputePitchShifting(totalDistance, rD->body) * rD->percentage;

			float doppler = ComputeDoppler(totalDistance, rD->body) * rD->percentage;

			float reverb = ComputeReverb(totalDistance, rD->body) * rD->percentage;

		}

		if (noVolume) continue;

		soundDataList->Add(new SoundData(aData->index, pan, volume, timeDelay));
	}
	agents->emitter->acousticDataList.Clear();
}

void Acoustics::CalculatePercentages(PhysArray<RayData*>* data, const float totalDistance)
{

}

void Acoustics::CalculatePercentagesVoidSecure(PhysArray<RayData*>* data, const float totalDistance)
{

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

float Acoustics::ComputeTimeDelay(float distance, Body* obstacle)
{
	// Calculate delay time
	// 1. Vel = sqrt( lambda * Pa / ro )
	// 2. Time = distance / vel

	float multiplier = 0;

	switch (obstacle->Class())
	{
	case BodyClass::DYNAMIC_BODY:
	{
		DynamicBody* solid = (DynamicBody*)obstacle;
		multiplier = solid->YoungModulus();
		break;
	}
	case BodyClass::STATIC_BODY:
	{
		StaticBody* solid = (StaticBody*)obstacle;
		multiplier = solid->YoungModulus();
		break;
	}

	case BodyClass::LIQUID_BODY:
	{
		LiquidBody* liquid = (LiquidBody*)obstacle;
		multiplier = liquid->BulkModulus();
		break;
	}

	case BodyClass::GAS_BODY:
	{
		GasBody* gas = (GasBody*)obstacle;
		multiplier = gas->HeatRatio() * gas->Pressure();
		break;
	}
	}

	float timeDelay = 0;
	float vel = PhysMath::Sqrt(multiplier / obstacle->Density(InUnit::IN_METERS));
	return distance / vel;
}

float Acoustics::ComputeVolumeAttenuation(float distance, Body* obstacle)
{
	float attenuation = 0;

	switch (obstacle->Class())
	{
	case BodyClass::DYNAMIC_BODY:
	case BodyClass::STATIC_BODY:
		attenuation = obstacle->AbsorptionCoefficient() * distance;
		break;

	case BodyClass::LIQUID_BODY:
	case BodyClass::GAS_BODY:
		attenuation = PhysMath::Exp(-obstacle->AbsorptionCoefficient() * distance);
		break;
	}

	return PhysMath::LogToLinear(attenuation, maxSPL) / maxVolume;
}

float Acoustics::ComputeFrequentialAttenuation(float distance, Body* obstacle)
{
	return 0;
}

float Acoustics::ComputePitchShifting(float distance, Body* obstacle)
{
	return 0;
}

float Acoustics::ComputeDoppler(float distance, Body* obstacle)
{
	return 0;
}

float Acoustics::ComputeReverb(float distance, Body* obstacle)
{
	return 0;
}

void Acoustics::RayCastBodyList(PhysArray<RayData*>* returnList, PhysRay ray)
{
	bodies->Iterate<PhysArray<RayData*>*, PhysRay, RaycastAgents*>
		(
			[](Body* b, PhysArray<RayData*>* ret, PhysRay ray, RaycastAgents* agents)
			{
				if (b == agents->listener || b == agents->emitter) return;

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
				if (PhysMath::CheckCollision(agents->listener->Rect(InUnit::IN_METERS), b->Rect(InUnit::IN_METERS))) outRay = { intr[0], agents->listener->ReceptionPoint(InUnit::IN_METERS)}; // Ray of the collision rectangles
				else outRay = { agents->emitter->EmissionPoint(InUnit::IN_METERS), intr[0]};
				ret->Add(new RayData(b, outRay, agents));

			},
			returnList,
			ray,
			agents
		);
}
