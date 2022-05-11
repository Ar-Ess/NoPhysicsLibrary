#ifndef __MODULE_H__
#define __MODULE_H__

#include "Log.h"
#include "Defs.h"

class Module
{
public:

	Module() : active(true)
	{}

	virtual ~Module()
	{
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool PreUpdate(float dt)
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	void Disable()
	{
		if (this && active)
		{
			active = false;
			CleanUp();
		}
	}

	void Enable()
	{
		if (this && !active)
		{
			active = true;
			Start();
		}
	}

public:

	bool active = true;

};

#endif // __MODULE_H__