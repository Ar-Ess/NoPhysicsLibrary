/*
No Physics Library

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software. This code is a variation from
the original Freeverb filter code.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.
*/

#include <math.h>
#include <string.h>
#include "soloud.h"
#include "NoReverbFilter.h"
#include "NoComb.h"
#include "NotAllpass.h"

namespace SoLoud
{
		namespace NoReverbImpl
		{
			// Based on code written by Jezar at Dreampoint, June 2000 http://www.dreampoint.co.uk, 
			// which was placed in public domain. The code was massaged quite a bit by 
			// Jari Komppa, result in the license listed at top of this file.

			const int	gNumcombs = 8;
			const int	gNumallpasses = 4;
			const float	gMuted = 0;
			const float	gFixedgain = 0.015f;
			const float gScalewet = 3;
			const float gScaledry = 2;
			const float gScaledamp = 0.4f;
			const float gScaleroom = 0.28f;
			const float gOffsetroom = 0.7f;
			const float gInitialroom = 0.5f;
			const float gInitialdamp = 0.5f;
			const float gInitialwet = 1 / gScalewet;
			const float gInitialdry = 0;
			const float gInitialwidth = 1;
			const float gInitialmode = 0;
			const float gFreezemode = 0.5f;
			const int	gStereospread = 23;

			int GetCombTunning(int x, Dir dir)
			{
				int spread = dir == LEFT ? 0 : gStereospread;
				return (int)floor((2650.5 + (1050.164 - 2650.5) / (1. + pow((x / 13.03415), 1.237097))) + spread);
			}

			int GetAllpassTunning(int x, bool left = true)
			{
				int spread = left ? 0 : gStereospread;
				return (int)floor((-476485900 + (664.7832 - -476485900) / pow(1 + (x / 4565876), 0.9965671)) + spread);
			}

			class Revmodel
			{
			public:

				Revmodel(int amount);

				void	mute();
				void	process(float* aSampleData, long aNumSamples);
				void	setroomsize(float aValue);
				void	setdamp(float aValue);
				void	setwet(float aValue);
				void	setdry(float aValue);
				void	setwidth(float aValue);
				void	setmode(float aValue);
				void	update();

				float	mGain;
				float	mRoomsize, mRoomsize1;
				float	mDamp, mDamp1;
				float	mWet, mWet1, mWet2;
				float	mDry;
				float	mWidth;
				float	mMode;

				int		mDirty;

				// The following are all declared inline 
				// to remove the need for dynamic allocation
				// with its subsequent error-checking messiness

				// Comb filters
				std::vector<NoComb>	mComb; // Size is equal to mBufComb length

				// Allpass filters
				std::vector<NotAllpass>	mAllpass; // Size is equal to mBufAllpass length

				// Buffers for the combs
				std::vector<std::vector<float>*> mBufComb; // 4 buffers? son 8 perquè cada un té L i R

				// Buffers for the allpasses
				std::vector<std::vector<float>*> mBufAllpass; // 4 buffers? son 8 perquè cada un té L i R
			};

			Revmodel::Revmodel(int amount)
			{
				mGain = 0;
				mRoomsize = 0;
				mRoomsize1 = 0;
				mDamp = 0;
				mDamp1 = 0;
				mWet = 0;
				mWet1 = 0;
				mWet2 = 0;
				mDry = 0;
				mWidth = 0;
				mMode = 0;

				mDirty = 1;

				const int combCount = amount;
				const int allpassCount = combCount / 2;

				mBufComb.reserve(combCount * 2);
				for (int i = 0; i < combCount; ++i)
				{
					std::vector<float>* ptr = new std::vector<float>;
					ptr->reserve(GetCombTunning(i + 1, LEFT));
					mBufComb.emplace_back(ptr);
					ptr = nullptr;
					ptr = new std::vector<float>;
					ptr->reserve(GetCombTunning(i + 1, RIGHT));
					mBufComb.emplace_back(ptr);
					ptr = nullptr;
				}

				mBufAllpass.reserve(allpassCount * 2);
				for (int i = 0; i < allpassCount; ++i)
				{
					std::vector<float>* ptr = new std::vector<float>;
					ptr->reserve(GetAllpassTunning(i + 1, LEFT));
					mBufAllpass.emplace_back(ptr);
					ptr = nullptr;
					ptr = new std::vector<float>;
					ptr->reserve(GetAllpassTunning(i + 1, RIGHT));
					mBufAllpass.emplace_back(ptr);
					ptr = nullptr;
				}

				mComb.reserve(combCount);
				// Tie the components to their buffers
				for (unsigned int i = 0; i < combCount; ++i)
				{
					mComb.emplace_back(NoComb());
					mComb[i].setbuffer(mBufComb[i * 2], GetCombTunning(i, LEFT), mBufComb[(i * 2) + 1], GetCombTunning(i, RIGHT));
				}

				mAllpass.reserve(allpassCount);
				for (unsigned int i = 0; i < allpassCount; ++i)
				{
					mComb.emplace_back(NoComb());
					mAllpass[i].setbuffer(mBufAllpass[i * 2], GetAllpassTunning(i, LEFT), mBufAllpass[(i * 2) + 1], GetAllpassTunning(i, RIGHT));
				}

				int size = mAllpass.size();
				for (unsigned int i = 0; i < size; ++i)
					mAllpass[i].setfeedback(0.5f, 0.5f);

				setwet(gInitialwet);
				setroomsize(gInitialroom);
				setdry(gInitialdry);
				setdamp(gInitialdamp);
				setwidth(gInitialwidth);
				setmode(gInitialmode);

				// Buffer will be full of rubbish - so we MUST mute them
				mute();
			}

			void Revmodel::mute()
			{
				if (mMode >= gFreezemode)
					return;

				for (int i = 0; i < gNumcombs; i++) mComb[i].mute();

				for (int i = 0; i < gNumallpasses; i++) mAllpass[i].mute();
			}

			// MAIN LOOP
			void Revmodel::process(float* aSampleData, long aNumSamples)
			{
				float* inputL, * inputR;
				inputL = aSampleData;
				inputR = aSampleData + aNumSamples;

				if (mDirty)
					update();
				mDirty = 0;

				while (aNumSamples-- > 0)
				{
					float outL, outR, input;
					outL = outR = 0;
					input = (*inputL + *inputR) * mGain;

					// Accumulate comb filters in parallel
					for (int i = 0; i < gNumcombs; i++)
					{
						outL += mComb[i].process(input, LEFT);
						outR += mComb[i].process(input, RIGHT);
					}

					// Feed through allpasses in series
					for (int i = 0; i < gNumallpasses; i++)
					{
						outL = mAllpass[i].process(outL, LEFT);
						outR = mAllpass[i].process(outR, RIGHT);
					}

					// Calculate output REPLACING anything already there
					*inputL = outL * mWet1 + outR * mWet2 + *inputL * mDry;
					*inputR = outR * mWet1 + outL * mWet2 + *inputR * mDry;

					// Increment sample pointers, allowing for interleave (if any)
					inputL++;
					inputR++;
				}
			}

			void Revmodel::update()
			{
				// UPDATE REVERB
				// Recalculate internal values after parameter change

				int i;

				mWet1 = mWet * (mWidth / 2 + 0.5f);
				mWet2 = mWet * ((1 - mWidth) / 2);

				// SOLOUD CHANGE
				//if (mMode >= gFreezemode)
				//{
				//	mRoomsize1 = 1;
				//	mDamp1 = 0;
				//	mGain = gMuted;
				//}
				//else
				//{
				mRoomsize1 = mRoomsize;
				mDamp1 = mDamp;
				mGain = gFixedgain;
				//}

				// REVERB ROOM SIZE PARAMETERS
				for (i = 0; i < gNumcombs; i++) mComb[i].setfeedback(mRoomsize1, mRoomsize1);

				for (i = 0; i < gNumcombs; i++) mComb[i].setdamp(mDamp1, mDamp1);
			}

			void Revmodel::setroomsize(float aValue)
			{
				// m_Roomsize = (value * 0.28f) + 0.7f
				mRoomsize = (aValue * gScaleroom) + gOffsetroom;
				mDirty = 1;
			}

			void Revmodel::setdamp(float aValue)
			{
				mDamp = aValue * gScaledamp;
				mDirty = 1;
			}

			void Revmodel::setwet(float aValue)
			{
				mWet = aValue * gScalewet;
				mDirty = 1;
			}

			void Revmodel::setdry(float aValue)
			{
				mDry = aValue * gScaledry;
			}

			void Revmodel::setwidth(float aValue)
			{
				mWidth = aValue;
				mDirty = 1;
			}

			void Revmodel::setmode(float aValue)
			{
				mMode = aValue;
				mDirty = 1;
			}
		}

		NoReverbFilterInstance::NoReverbFilterInstance(NoReverbFilter* aParent)
		{
			initParams(5);

			mParent = aParent;

			mModel = new NoReverbImpl::Revmodel(aParent->mAmount);
			mModel->mDry = 1 - aParent->wet;
			mModel->mWet = aParent->wet;
			mParam[FREEZE] = aParent->mMode;
			mParam[ROOMSIZE] = aParent->mRoomSize;
			mParam[DAMP] = aParent->mDamp;
			mParam[WIDTH] = aParent->mWidth;
			mParam[WET] = aParent->wet;
		}

		void NoReverbFilterInstance::filter(float* aBuffer, unsigned int aSamples, unsigned int aChannels, float aSamplerate, time aTime)
		{
			//SOLOUD_ASSERT(aChannels == 2); // Only stereo supported at this time

			if (mParamChanged)
			{
				mModel->setdamp(mParam[DAMP]);
				mModel->setmode(mParam[FREEZE]);
				mModel->setroomsize(mParam[ROOMSIZE]);
				mModel->setwidth(mParam[WIDTH]);
				mModel->setwet(mParam[WET]);
				mModel->setdry(1 - mParam[WET]);
				mParamChanged = 0;
			}

			mModel->process(aBuffer, aSamples);
		}

		NoReverbFilterInstance::~NoReverbFilterInstance()
		{
			delete mModel;
		}

		NoReverbFilter::NoReverbFilter(int amount)
		{
			if (amount < 2) amount = 2;
			if (amount % 2) ++amount;
			mAmount = amount;
			setParams(0.5, 0.5, 0.5, 1);
		}

		result NoReverbFilter::setParams(float drywet, float aRoomSize, float aDamp, float aWidth)
		{
			if (drywet < 0 || drywet > 1 || aRoomSize <= 0 || aDamp < 0 || aWidth <= 0)
				return INVALID_PARAMETER;

			mMode = 0;
			mRoomSize = aRoomSize;
			mDamp = aDamp;
			mWidth = aWidth;
			this->wet = drywet;

			return 0;
		}

		int NoReverbFilter::getParamCount()
		{
			return 5;
		}

		const char* NoReverbFilter::getParamName(unsigned int aParamIndex)
		{
			switch (aParamIndex)
			{
			case FREEZE: return "Freeze";
			case ROOMSIZE: return "Room size";
			case DAMP: return "Damp";
			case WIDTH: return "Width";
			}
			return "Wet";
		}

		unsigned int NoReverbFilter::getParamType(unsigned int aParamIndex)
		{
			if (aParamIndex == FREEZE)
				return BOOL_PARAM;
			return FLOAT_PARAM;
		}

		float NoReverbFilter::getParamMax(unsigned int aParamIndex)
		{
			return 1;
		}

		float NoReverbFilter::getParamMin(unsigned int aParamIndex)
		{
			return 0;
		}

		NoReverbFilter::~NoReverbFilter()
		{
		}

		NoReverbFilterInstance* NoReverbFilter::createInstance()
		{
			return new NoReverbFilterInstance(this);
		}

}
