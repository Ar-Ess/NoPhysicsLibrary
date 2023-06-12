#pragma once

class PhysTrigger
{

	friend class NPL;

public:

	// Operator to check if the trigger has been activated
	// Usage: if (myTrigger()) return;
	bool operator()()
	{
		if (!trigger) return true;

		trigger = false;
		return false;
	}

	// Operator to activate the trigger
	// Usage: !myTrigger;
	void operator!()
	{
		trigger = true;
	}

	// Returns whether the trigger is active or not
	// It is recommended to use the () operator instead of this one
	// Usage: if (!myTrigger.IsTriggered()) return;
	bool IsTriggered()
	{
		if (!trigger) return false;

		trigger = false;
		return true;
	}

	// Activates the trigger
	// Usage: myTrigger.Trigger();
	// It is recommended to use the ! operator instead of this one
	void Trigger()
	{
		trigger = true;
	}

private:

	bool trigger = false;

};
