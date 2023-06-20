#include "Acoustics.h"
#include "SoundData.h"
#include "GasBody.h"
#include "DynamicBody.h"
#include "StaticBody.h"
#include "LiquidBody.h"
#include "PhysArray.h"

#define MAX_FREQ 22000
#define	FINAL_FREQ(x) x * MAX_FREQ
#define FINAL_RES(x) ((x + (0.2f * x)) / 1.2f) * 2
#define REF_VEL 343
#define FINAL_PITCH(x, factor) (factor * x) + ((1 - factor) * 1)

Acoustics::RayData::RayData(Body* body, PhysRay ray, Acoustics::RaycastAgents* agents)
{
	this->body = body;
	this->innerDistance = ray.Distance();
	if (agents != nullptr) this->distance = PhysMath::Distance(agents->emitter->rect.Position(), ray.start);
	else this->distance = 0;

	this->ray = ray;
}

bool Acoustics::RayData::operator==(RayData data) const
{
	return body == data.body;
}

Acoustics::Acoustics(PhysArray<Body*>* bodies, PhysArray<SoundData*>* soundDataList, PhysArray<unsigned int*>* gasIndex, PhysArray<unsigned int*>* liquidIndex, const float* panRange, const float* panFactor, const float* pitchVariationFactor, const Flag* generalConfig, const float* globalVolumeAttFactor)
{
	this->bodies = bodies;
	this->soundDataList = soundDataList;
	this->panRange = panRange;
	this->panFactor = panFactor;
	this->pitchVariationFactor = pitchVariationFactor;
	this->gasIndex = gasIndex;
	this->liquidIndex = liquidIndex;
	this->agents = new RaycastAgents();
	this->generalConfig = generalConfig;
	this->globalVolumeAttFactor = globalVolumeAttFactor;
}

void Acoustics::Simulate(Body* emitter, Body* listener)
{
	if (!listener || listener->id == emitter->id)
	{
		emitter->acousticDataList.Clear();
		return;
	}

	agents->SetAgents(emitter, listener);

	GasBody* environment = GetEnvironmentBody();
	LiquidBody* flood = GetFloodBody();
	if (environment == nullptr && flood == nullptr) return emitter->acousticDataList.Clear(); // This assures us that the emitter is inside a gas or liquid

	PhysRay ray = PhysRay(emitter->EmissionPoint(InUnit::IN_METERS), listener->ReceptionPoint(InUnit::IN_METERS));
	const float totalDistance = PhysMath::Distance(ray);

	PhysArray<RayData*> data;
	RayCastBodyList(&data, ray);

	data.Sort([](RayData* a, RayData* b) { return a->distance > b->distance; });

	RayData find1 = RayData(environment, PhysRay(), nullptr);
	RayData find2 = RayData(flood, PhysRay(), nullptr);

	if (!VoidSecurityLogic(&data, environment, flood, totalDistance))
	{
		emitter->acousticDataList.Clear();
		return;
	}

	ListenerLogic(&data, totalDistance);

}

void Acoustics::NoListenerLogic(Body* emmiter)
{
	emmiter->acousticDataList.Iterate<PhysArray<SoundData*>*, const float>
	(
		[](AcousticData* data, PhysArray<SoundData*>* soundDataList, const float maxSPL)
		{
			soundDataList->Add(new SoundData(data->index, 0, data->spl / maxSPL, 0, 22000, 2, 1));
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

		if (volume < 0.005) continue;

		float timeDelay = 0;
		float cutoff = 1;
		float res = 1;
		float pitch = 1;
		bool noVolume = false;
		bool freqAttenuation = generalConfig->Get(3);
		bool soundOclusion = generalConfig->Get(4);

		for (unsigned int i = 0; i < data->Size(); ++i)
		{
			RayData* rD = data->At(i);
			if (!(agents->listener->Class() == BodyClass::DYNAMIC_BODY && // If body is excluded from collision, don't affect
				((DynamicBody*)agents->listener)->IsIdExcludedFromCollision(rD->body))) continue;


			if (soundOclusion)
				volume -= ComputeVolumeAttenuation(rD->innerDistance, rD->body);
			noVolume = volume < 0.01f;
			if (noVolume) break;

			float velocity = 0;
			timeDelay += ComputeTimeDelay(rD->innerDistance, rD->body, velocity);

			if (freqAttenuation)
				ComputeFrequentialAttenuation(rD->innerDistance, totalDistance, velocity, rD->body, cutoff, res);

			ComputePitchShifting(velocity, rD->innerDistance / totalDistance, pitch);

			//float doppler = ComputeDoppler(totalDistance, rD->body) * rD->percentage;

			//float reverb = ComputeReverb(totalDistance, rD->body) * rD->percentage;

		}

		if (noVolume) continue;

		if ((pitch > 1 && !generalConfig->Get(1)) || (pitch < 1 && !generalConfig->Get(2))) pitch = 1;
		if (!generalConfig->Get(5)) timeDelay = 0;

		soundDataList->Add(new SoundData(aData->index, pan, volume, timeDelay, FINAL_FREQ(cutoff), FINAL_RES(res), FINAL_PITCH(pitch, *pitchVariationFactor)));
	}
	agents->emitter->acousticDataList.Clear();
}

bool Acoustics::VoidSecurityLogic(PhysArray<RayData*>* data, GasBody* environment, LiquidBody* flood, const float totalDistance)
{
	RayData find1 = RayData(environment, PhysRay(), nullptr);
	RayData find2 = RayData(flood, PhysRay(), nullptr);

	// INFO: LiquidBodies properties takes priority over gas bodies
	if (flood != nullptr && !data->Contains(&find2))
	// If the flood liquid isn't contained, it means that it englobes the agents, taking priority over gas
	{
		data->Add(new RayData(flood, PhysRay(agents->emitter->EmissionPoint(InUnit::IN_METERS), agents->listener->ReceptionPoint(InUnit::IN_METERS)), nullptr));
		return true;
	}

	else if (environment != nullptr && !data->Contains(&find1))
	// If the enviroment gas isn't contained, it means that it englobes the agents
	{
		data->Add(new RayData(environment, PhysRay(agents->emitter->EmissionPoint(InUnit::IN_METERS), agents->listener->ReceptionPoint(InUnit::IN_METERS)), nullptr));
		return true;
	}

	// If the enviroment gas and flood liquid are contained, it means that any of both englobe the agents and there is a probability of void	
	// INFO: This system is incomplete. It does not detect and solve overlapping of bodies
	float distTraveled = 0;
	bool outVoid = false;

	data->Iterate<float&, const float, bool&>
	(
		[](RayData* data, float& distTraveled, const float totalDist, bool& outVoid)
		{
			if (distTraveled >= totalDist || outVoid) return;

			if (data->distance == 0)
			{
				distTraveled += data->innerDistance;
				return;
			}

			if (data->distance <= distTraveled)
			{
				if (data->distance + data->innerDistance > distTraveled)
					distTraveled += (data->distance + data->innerDistance - distTraveled);
			}
			else
				outVoid = true;
		},
		distTraveled,
		totalDistance,
		outVoid
	);

	if (distTraveled < totalDistance) outVoid = true;

	return !outVoid;
}

GasBody* Acoustics::GetEnvironmentBody()
{
	PhysRect rect = PhysRect(float(agents->emitter->EmissionPoint(InUnit::IN_METERS).x), float(agents->emitter->EmissionPoint(InUnit::IN_METERS).y), 1.0f, 1.0f);
	PhysArray<GasBody*> possibles;
	for (unsigned int i = 0; i < gasIndex->Size(); ++i)
	{
		Body* b = bodies->At(*(*gasIndex)[i]);
		if (PhysMath::CheckCollision(rect, b->Rect(InUnit::IN_METERS)))
			possibles.Add((GasBody*)b);
	}

	if (possibles.Empty()) return nullptr;
	if (possibles.Size() < 2) return possibles[0];

	possibles.Sort([](GasBody* a, GasBody* b) { return a->Rect(InUnit::IN_METERS).Area() < b->Rect(InUnit::IN_METERS).Area(); });

	return possibles[0];
}

LiquidBody* Acoustics::GetFloodBody()
{
	PhysRect rect = PhysRect(float(agents->emitter->EmissionPoint(InUnit::IN_METERS).x), float(agents->emitter->EmissionPoint(InUnit::IN_METERS).y), 1.0f, 1.0f);
	PhysArray<LiquidBody*> possibles;
	for (unsigned int i = 0; i < liquidIndex->Size(); ++i)
	{
		Body* b = bodies->At(*(*liquidIndex)[i]);
		if (PhysMath::CheckCollision(rect, b->Rect(InUnit::IN_METERS)))
			possibles.Add((LiquidBody*)b);
	}

	if (possibles.Empty()) return nullptr;
	if (possibles.Size() < 2) return possibles[0];

	possibles.Sort([](LiquidBody* a, LiquidBody* b) { return a->Rect(InUnit::IN_METERS).Area() < b->Rect(InUnit::IN_METERS).Area(); });

	return possibles[0];
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
	float fSPL = spl - (20 * PhysMath::Log(distance * *globalVolumeAttFactor * *globalVolumeAttFactor));

	// Transform volume from db to linear [ 0, 1])
	return PhysMath::LogToLinear(fSPL, maxSPL) / maxVolume;
}

float Acoustics::ComputeTimeDelay(float distance, Body* obstacle, float& outVelocity)
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
		multiplier = solid->YoungModulus() * 1000000000.0;
		break;
	}

	case BodyClass::LIQUID_BODY:
	{
		LiquidBody* liquid = (LiquidBody*)obstacle;
		multiplier = liquid->BulkModulus() * 1000000000.0;
		break;
	}

	case BodyClass::GAS_BODY:
	{
		GasBody* gas = (GasBody*)obstacle;
		multiplier = gas->HeatRatio() * gas->Pressure();
		break;
	}
	}

	outVelocity = PhysMath::Sqrt(multiplier / obstacle->Density(InUnit::IN_METERS));
	return distance / outVelocity;
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

void Acoustics::ComputeFrequentialAttenuation(float innerDistance, float distance, float velocity, Body* obstacle, float& outCutoff, float& outResonance)
{
	switch (obstacle->Class())
	{
	case BodyClass::LIQUID_BODY:
	{
		const LiquidBody* liquid = (LiquidBody*)obstacle;
		const float percent = innerDistance / distance;
		const float steps = 20;
		const float width = MAX_FREQ / steps;

		float ret = 1;
		float totalArea = 0;

		for (unsigned int i = 0; i < steps; ++i)
		{
			float visc = liquid->Viscosity() / liquid->Density(InUnit::IN_METERS);
			float height = PhysMath::Exp(-(2 * PhysMath::Pi() * width * i * visc));
			totalArea += width * height;
		}

		if (totalArea > MAX_FREQ) totalArea = MAX_FREQ;
		ret = totalArea / MAX_FREQ;
		ret = (ret * percent) + (outCutoff * (1 - percent));

		outCutoff = PhysMath::Min(ret, outCutoff);
		outResonance = PhysMath::Min(ret, outResonance);

		break;
	}

	case BodyClass::DYNAMIC_BODY:
	case BodyClass::STATIC_BODY:
	{
		float waveLength = 2 * (innerDistance / obstacle->AbsorptionCoefficient());
		float ret = (velocity / waveLength) / MAX_FREQ;
		if (ret > 1) ret = 1;

		outCutoff = PhysMath::Min(ret, outCutoff);

		break;
	}
	}

}

void Acoustics::ComputePitchShifting(float velocity, const float percent, float& outPitch)
{
	float pitch = velocity / REF_VEL;
	const float relPercent = 1 - percent;

	outPitch = (outPitch * (relPercent)) + (pitch * percent);
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
				bool distZero = true;
				if (PhysMath::CheckCollision(agents->listener->Rect(InUnit::IN_METERS), b->Rect(InUnit::IN_METERS)))
				{
					distZero = false;
					outRay = { intr[0], agents->listener->ReceptionPoint(InUnit::IN_METERS) }; // Ray of the collision rectangles
				}
				else outRay = { agents->emitter->EmissionPoint(InUnit::IN_METERS), intr[0]};
				ret->Add(new RayData(b, outRay, distZero ? nullptr : agents));

			},
			returnList,
			ray,
			agents
		);
}
