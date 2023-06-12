#pragma once

class Material
{
public:

	Material()
	{
		youngModulus = 0;
		viscosity = 0;
		bulkModulus = 0;
		heatRatio = 1.414f;
		density = 0;
		defaultTemperature = 0;
		absorptionCoefficient = 0.01;
	}
	
	// Build the material for solids
	// -1 does not modify the value
	void BuildSolid(float youngModulus, float absorptionCoefficient, float density, float defaultTemperature);
	
	// Build the material for liquids
	// -1 does not modify the value
	void BuildLiquid(float viscosity, float bulkModulus, float absorptionCoefficient, float density, float defaultTemperature);

	// Build the material for gases
	// -1 does not modify the value
	void BuildGas(float heatRatio, float absorptionCoefficient, float density, float defaultTemperature);

private:

	friend class Body;
	friend class DynamicBody;
	friend class StaticBody;
	friend class GasBody;
	friend class LiquidBody;

	float youngModulus = 0;
	float viscosity = 0;
	float bulkModulus = 0;
	float heatRatio = 1.414f;
	float density = 0;
	float defaultTemperature = 0;
	float absorptionCoefficient = 0.01;
	bool defaults = true;

};
