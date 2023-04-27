#include "Material.h"

void Material::BuildSolid(float youngModulus, float density, float defaultTemperature)
{
	this->youngModulus = youngModulus;
	this->density = density;
	this->temperature = defaultTemperature;
	defaults = false;
}

void Material::BuildLiquid(float viscosity, float bulkModulus, float density, float defaultTemperature)
{
	this->viscosity = viscosity;
	this->bulkModulus = bulkModulus;
	this->density = density;
	this->temperature = defaultTemperature;
	defaults = false;
}

void Material::BuildGas(float heatRatio, float density, float defaultTemperature)
{
	this->heatRatio = heatRatio;
	this->density = density;
	this->temperature = defaultTemperature;
	defaults = false;
}
