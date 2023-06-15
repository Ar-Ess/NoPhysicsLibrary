#include "Material.h"

void Material::BuildSolid(float youngModulus, float absorptionCoefficient, float density, float defaultTemperature)
{
	this->youngModulus = youngModulus == -1 ? this->youngModulus : youngModulus;
	this->defaultTemperature = defaultTemperature == -1 ? this->defaultTemperature : defaultTemperature;
	this->absorptionCoefficient = absorptionCoefficient == -1 ? this->absorptionCoefficient : absorptionCoefficient;
	
	this->density = density;
}

void Material::BuildLiquid(float viscosity, float bulkModulus, float absorptionCoefficient, float density, float defaultTemperature)
{
	this->viscosity = viscosity == -1 ? this->viscosity : viscosity;
	this->bulkModulus = bulkModulus == -1 ? this->bulkModulus : bulkModulus;
	this->defaultTemperature = defaultTemperature == -1 ? this->defaultTemperature : defaultTemperature;
	this->absorptionCoefficient = absorptionCoefficient == -1 ? this->absorptionCoefficient : absorptionCoefficient;
	
	this->density = density;
}

void Material::BuildGas(float heatRatio, float pressure, float absorptionCoefficient, float density, float defaultTemperature)
{
	this->heatRatio = heatRatio == -1 ? this->heatRatio : heatRatio;
	this->pressure = pressure == -1 ? this->pressure : pressure;
	this->defaultTemperature = defaultTemperature == -1 ? this->defaultTemperature : defaultTemperature;
	this->absorptionCoefficient = absorptionCoefficient == -1 ? this->absorptionCoefficient : absorptionCoefficient;
	
	this->density = density;
}
