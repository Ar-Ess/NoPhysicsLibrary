#pragma once

#include <stdint.h>

class ID
{
public:

	ID(bool empty = false)
	{
		if (!empty) id = reinterpret_cast<intptr_t>(this);
		this->empty = empty;
	}

	bool operator==(ID id)
	{
		if (empty || id.empty) return empty && id.empty;

		return this->id == id.id;
	}

	void operator=(ID id)
	{
		this->id = id.id;
		empty = id.empty;
	}

	bool Empty() const
	{
		return empty;
	}

	bool Clear()
	{
		empty = true;
		return true;
	}

private:

	intptr_t id = 0;
	bool empty = false;

};
