#pragma once

class Material
{
public:

	Material()
	{
		youngModulus = 100;
		viscosity = 0.001;
		bulkModulus = 2.1;
		heatRatio = 1.414f;
		density = -1;
		absorptionCoefficient = 0.01;
		pressure = 1000;
	}
	
	// Build the material for solids [set -1 to not modify the value]
	// Young Modulus [GPa] | Absrpt Coeff [1/m] | Density [Kg/m2]
	void BuildSolid(float youngModulus, float absorptionCoefficient, float density);
	
	// Build the material for liquids [set -1 to not modify the value]
	// Viscosity [Pa/s] | Bulk Modulus [GPa] | Absrpt Coeff [1/m] | Density [Kg/m2]
	void BuildLiquid(float viscosity, float bulkModulus, float absorptionCoefficient, float density);

	// Build the material for gases [set -1 to not modify the value]
	// Heat Ratio [None] | Absrpt Coeff [1/m] | Density [Kg/m2]
	void BuildGas(float heatRatio, float pressure, float absorptionCoefficient, float density);

private:

	friend class Body;
	friend class DynamicBody;
	friend class StaticBody;
	friend class GasBody;
	friend class LiquidBody;

	// INFO: young modulus default is wood [in gigaPascals]
	float youngModulus = 100;

	// INFO: viscosity default is water [in pascals/second]
	float viscosity = 0.001;

	// INFO: bulk modulus default is water [in gigaPascals]
	float bulkModulus = 2.1;

	// INFO: heatRatio default is air [no unit]
	float heatRatio = 1.414f;

	// INFO: temperature default is 10C� [Kelvin]
	/*float defaultTemperature = 283;*/

	// INFO: absrpt coeff default is water [1/meter]
	float absorptionCoefficient = 0.01;

	// INFO: pressure default is air [Pa]
	float pressure = 1000;
	
	float density = -1;

};
