#pragma once

#include <vector>

class NotAllpass
{
	struct Allpass
	{
		Allpass()
		{
			mBufidx = 0;
			mFeedback = 0;
			mBuffer = 0;
			mBufsize = 0;
		}

		float	mFeedback;
		std::vector<float>* mBuffer = nullptr;
		int		mBufsize;
		int		mBufidx;
	};

public:

	NotAllpass()
	{
		allpass[LEFT] = Allpass();
		allpass[RIGHT] = Allpass();
	}

	void setbuffer(std::vector<float>* aBufL, int aSizeL, std::vector<float>* aBufR, int aSizeR)
	{
		allpass[LEFT].mBuffer = aBufL;
		allpass[LEFT].mBufsize = aSizeL;

		allpass[RIGHT].mBuffer = aBufR;
		allpass[RIGHT].mBufsize = aSizeR;
	}

	float process(float aInput, Dir dir)
	{
		float output;
		float bufout;

		bufout = (*allpass[(int)dir].mBuffer)[allpass[(int)dir].mBufidx];

		output = -aInput + bufout;
		(*allpass[(int)dir].mBuffer)[allpass[(int)dir].mBufidx] = aInput + (bufout * allpass[(int)dir].mFeedback);

		if (++(allpass[(int)dir].mBufidx) >= allpass[(int)dir].mBufsize) 
			allpass[(int)dir].mBufidx = 0;

		return output;
	}

	void mute()
	{
		for (int i = 0; i < allpass[LEFT].mBufsize; i++)
			(*allpass[LEFT].mBuffer)[i] = 0;

		for (int i = 0; i < allpass[RIGHT].mBufsize; i++)
			(*allpass[RIGHT].mBuffer)[i] = 0;
	}

	// Set -1 in any value to not modify the parameter
	void setfeedback(float aValL, float aValR)
	{
		if (aValL != -1) allpass[LEFT].mFeedback = aValL;
		if (aValR != -1) allpass[RIGHT].mFeedback = aValR;
	}

private:

	Allpass allpass[2];

};
