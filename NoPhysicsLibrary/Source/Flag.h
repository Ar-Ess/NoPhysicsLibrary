#pragma once

#include <assert.h>
#include <stdint.h>

// 8 slots bit-wise manager
class Flag
{
public:

	Flag()
	{

	}

	Flag(uint8_t flag)
	{
		this->flag = flag;
	}

	// Returns the binary flags in a number of 8 bits
	uint8_t Binary() const
	{
		return flag;
	}

	// Set an specific flag from slot 0 to 7
	void Set(short int index, bool state)
	{
		// Flags only have 8 slots available
		assert(index >= 0 && index < 8);

		if (Get(index) == state) return;

		uint8_t mod = 1;
		mod = mod << index;

		flag = flag ^ mod;
	}

	// Set the whole flag
	inline void Set(uint8_t flag) { this->flag = flag; }

	// Get an specific flag
	bool Get(short int index) const
	{
		// Flags only have 8 slots available
		assert(index >= 0 && index < 8);

		uint8_t mod = 1;
		mod = mod << index;

		mod = flag & mod;

		return (mod != 0);
	}

	// Inverts the state of all the flags
	inline void Invert() { flag = ~flag; }

	inline bool IsAnyTrue() const { return flag != 0; };

	inline void Clear() { flag = 0; };

private:

	uint8_t flag = 0;

};

