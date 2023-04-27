#pragma once

class NoMaterial
{
public:

	NoMaterial()
	{
		youngModulus = 0;
		viscosity = 0;
		bulkModulus = 0;
		heatRatio = 0;
		density = 0;
		defaultTemperature = 0;
	}
	
	// Build the material for solids
	// -1 does not modify the value
	void BuildSolid(float youngModulus, float density, float defaultTemperature);
	
	// Build the material for liquids
	// -1 does not modify the value
	void BuildLiquid(float viscosity, float bulkModulus, float density, float defaultTemperature);

	// Build the material for gases
	// -1 does not modify the value
	void BuildGas(float heatRatio, float density, float defaultTemperature);

private:

	float youngModulus = 0;
	float viscosity = 0;
	float bulkModulus = 0;
	float heatRatio = 0;
	float density = 0;
	float defaultTemperature = 0;

};
