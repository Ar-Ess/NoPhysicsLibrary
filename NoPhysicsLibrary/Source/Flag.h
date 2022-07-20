#pragma once

#include <assert.h>
#include <stdint.h>

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

	// Set an specific flag from slot 0 to 7
	void Set(bool state, short int index)
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
	bool Get(short int index)
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

private:

	uint8_t flag = 0;

};

