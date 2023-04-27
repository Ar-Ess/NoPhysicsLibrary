#include "NoMaterial.h"

void NoMaterial::BuildSolid(float youngModulus, float density, float defaultTemperature)
{
	this->youngModulus = youngModulus;
	this->density = density;
	this->defaultTemperature = defaultTemperature;
}

void NoMaterial::BuildLiquid(float viscosity, float bulkModulus, float density, float defaultTemperature)
{
	this->viscosity = viscosity;
	this->bulkModulus = bulkModulus;
	this->density = density;
	this->defaultTemperature = defaultTemperature;
}

void NoMaterial::BuildGas(float heatRatio, float density, float defaultTemperature)
{
	this->heatRatio = heatRatio;
	this->density = density;
	this->defaultTemperature = defaultTemperature;
}
