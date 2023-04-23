#pragma once

enum Dir
{
	LEFT = 0,
	RIGHT = 1
};

#include <vector>

class NoComb
{

	struct Comb
	{
		Comb()
		{
			mFilterstore = 0;
			mBufidx = 0;
			mFeedback = 0;
			mDamp = 0;
			mDampInv = 0;
			mBuffer = 0;
			mBufsize = 0;
		}

		float mFeedback;
		float mFilterstore;
		float mDamp;
		float mDampInv;
		std::vector<float>* mBuffer = nullptr;
		int	mBufsize;
		int	mBufidx;
	};

public:

	NoComb()
	{
		combs[LEFT] = Comb();
		combs[RIGHT] = Comb();
	}

	void setbuffer(std::vector<float>* aBufL, int aSizeL, std::vector<float>* aBufR, int aSizeR)
	{
		combs[LEFT].mBuffer = aBufL;
		combs[LEFT].mBufsize = aSizeL;
		combs[RIGHT].mBuffer = aBufR;
		combs[RIGHT].mBufsize = aSizeR;
	}

	float process(float aInput, Dir dir)
	{
		float output;

		output = (*combs[(int)dir].mBuffer)[combs[(int)dir].mBufidx];

		// NEXT CLASS
		combs[(int)dir].mFilterstore = (output * combs[(int)dir].mDampInv) + (combs[(int)dir].mFilterstore * combs[(int)dir].mDamp);

		(*combs[(int)dir].mBuffer)[combs[(int)dir].mBufidx] = aInput + (combs[(int)dir].mFilterstore * combs[(int)dir].mFeedback);

		if (++(combs[(int)dir].mBufidx) >= combs[(int)dir].mBufsize) combs[(int)dir].mBufidx = 0;

		return output;
	}

	void mute()
	{
		for (int i = 0; i < combs[RIGHT].mBufsize; i++)
			(*combs[RIGHT].mBuffer)[i] = 0;

		for (int i = 0; i < combs[LEFT].mBufsize; i++)
			(*combs[LEFT].mBuffer)[i] = 0;
	}

	// Set -1 in any value to not modify the parameter
	void setdamp(float aValL, float aValR)
	{
		if (aValL != -1)
		{
			combs[LEFT].mDamp = aValL;
			combs[LEFT].mDampInv = 1 - aValL;
		}

		if (aValR != -1)
		{
			combs[RIGHT].mDamp = aValR;
			combs[RIGHT].mDampInv = 1 - aValR;
		}
	}

	// Set -1 in any value to not modify the parameter
	void setfeedback(float aValL, float aValR)
	{
		if (aValL != -1) combs[LEFT].mFeedback = aValL;
		if (aValR != -1) combs[RIGHT].mFeedback = aValR;
	}

private:

	Comb combs[2];
};