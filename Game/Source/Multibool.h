#ifndef __MULTIBOOL_H__
#define __MULTIBOOL_H__

#include <math.h>

class Multibool
{
public:

	Multibool(int amount)
	{
		if (amount > 1 || amount < 9)
		{
			this->amount = amount;
			bools = pow(10, amount);
			bools *= 2;
			allFalse = bools;
			for (unsigned short int i = 0; i < amount; i++) allTrue += pow(10, i);
			allTrue += bools;
		}
	}

	unsigned short int GetAmount() const
	{
		return amount;
	}

	void Set(int index, bool set)
	{
		// index out of bounds
		assert(index >= 0 || index < amount);
		if (Get(index) == set) return;

		int change = pow(10, index);
		set ? change *= 1 : change *= -1;

		bools += change;

		return;
	}

	void SetAllFalse()
	{
		bools = allFalse;
	}

	bool Get(int index)
	{
		assert(index >= 0 || index < amount);

		int result = bools / pow(10, index);

		result = result % 10;

		return result;
	}

	bool IsAllTrue() const
	{
		return (bools - allTrue == 0);
	}

	bool IsAllFalse() const
	{
		return (bools - allFalse == 0);
	}

private:

	unsigned int bools = 0;
	unsigned short int amount = 0;
	unsigned short int allFalse = 0;
	unsigned short int allTrue = 0;
};

#endif  // __MULTIBOOL_H__
