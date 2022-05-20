#ifndef __FLAGS_H__
#define __FLAGS_H__

#include <assert.h>

class Flag
{
public:
	Flag()
	{

	}

	// Set an specific flag
	void Set(short int index, bool state)
	{
		// Flags only have 8 slots available
		assert(index >= 0 && index < 8);

		if (Get(index) == state) return;

		uint8_t mod = 1;
		mod = mod << index;

		flag = flag ^ mod;
	}

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
	void Invert()
	{
		flag = ~flag;
	}

	uint8_t GetNumber() const
	{
		return flag;
	}

private:

	uint8_t flag = 0;

};

#endif // !__FLAGS_H__

