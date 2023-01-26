#pragma once

#include <assert.h>
#include <stdint.h>

// 64 slots bit-wise manager
class Flag
{
public:

	Flag()
	{
		this->flag = 0;
	}

	Flag(uint64_t flag)
	{
		this->flag = flag;
	}

	// Returns the binary flags in a number of 8 bits
	uint64_t Binary() const
	{
		return flag;
	}

	// Set an specific flag from slot 0 to 63
	void Set(unsigned int index, bool state)
	{
		// Flags only have 64 slots available
		assert(index < capacity);

		if (Get(index) == state) return;

		uint64_t mod = 1;
		mod = mod << index;

		flag = flag ^ mod;
	}

	// Set the whole flag
	void Set(uint64_t flag) { this->flag = flag; }

	// Get an specific flag
	bool Get(unsigned int index) const
	{
		// Flags only have 64 slots available
		assert(index < capacity);

		uint64_t mod = 1;
		mod = mod << index;

		mod = flag & mod;

		return (mod != 0);
	}

	// Get and specific flag. If the flag is true, it sets it to false
	bool Trigger(unsigned int index)
	{
		// Flags only have 64 slots available
		assert(index < capacity);

		uint64_t mod = 1;
		mod = mod << index;

		mod = flag & mod;

		bool ret = (mod != 0);
		if (ret) flag = flag ^ mod;

		return ret;
	}

	// Inverts the state of all the flags
	void Invert() { flag = ~flag; }

	bool IsAnyTrue() const { return flag != 0; };

	void Clear() { flag = 0; };

	int Capacity() const { return capacity; }

private:

	uint64_t flag = 0;
	const uint8_t capacity = 64;
};

