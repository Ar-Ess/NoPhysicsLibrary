#pragma once

#include "External/SoLoud/include/soloud_fftfilter.h"
#include "External/SoLoud/include/soloud_filter.h"

class PitchShiftFilter;
class FFTFilter;

class PitchShiftFilterInstance : public SoLoud::FFTFilterInstance
{
public:

    PitchShiftFilterInstance(SoLoud::FFTFilter* aParent, float pitch) : SoLoud::FFTFilterInstance(aParent)
    {
        this->pitch = pitch;
    }

    PitchShiftFilterInstance()
    {

    }

    void filter(float* aBuffer, unsigned int aSamples, unsigned int aChannels, float aSamplerate, SoLoud::time aTime) override
    {
        //pitch = powf(2.0f, pitch / 12.0f); (In case we want it for semitones)

        if (pitch == 1) return;

        // Call the parent class's filter function
        SoLoud::FFTFilterInstance::filter(aBuffer, aSamples, aChannels, aSamplerate, aTime);

        // Apply the pitch shift to the audio samples
        for (unsigned int i = 0; i < aSamples * aChannels; ++i)
        {
            //// Linear Interpolation Low Accuracy
            //// Interpolate between adjacent samples to shift the pitch
            //float index = i * pitch;
            //int index1 = static_cast<int>(index);
            //int index2 = index1 + 1;
            //float fraction = index - index1;
            //// Perform linear interpolation
            //aBuffer[i] = (1.0f - fraction) * aBuffer[index1] + fraction * aBuffer[index2];

            //Cubic Interpolation High Accuracy
            float index = 0;
            if (pitch > 1) index = i * pitch;
            else index = i / pitch;
            int index1 = static_cast<int>(index);
            float fraction = index - index1;
            // Perform cubic interpolation
            int index0 = index1 - 1;
            int index2 = index1 + 1;
            int index3 = index1 + 2;
            float value0 = (index0 >= 0 && index0 < aSamples* aChannels) ? aBuffer[index0] : 0.0f;
            float value1 = (index1 >= 0 && index1 < aSamples* aChannels) ? aBuffer[index1] : 0.0f;
            float value2 = (index2 >= 0 && index2 < aSamples* aChannels) ? aBuffer[index2] : 0.0f;
            float value3 = (index3 >= 0 && index3 < aSamples* aChannels) ? aBuffer[index3] : 0.0f;
            float t = fraction;
            float t2 = t * t;
            float t3 = t2 * t;
            // Cubic interpolation equation
            float interpolatedValue = 0.5f * ((2.0f * value1) + (-value0 + value2) * t + (2.0f * value0 - 5.0f * value1 + 4.0f * value2 - value3) * t2 + (-value0 + 3.0f * value1 - 3.0f * value2 + value3) * t3);
            // Assign the interpolated value to the current sample
            aBuffer[i] = interpolatedValue;
        }
    }

private:

    float pitch = 1;

};

class PitchShiftFilter : public SoLoud::FFTFilter
{
public:

    void SetPitchShift(float pitch)
    {
        this->pitch = pitch;
    }

    SoLoud::FilterInstance* createInstance() override
    {
        return new PitchShiftFilterInstance((SoLoud::FFTFilter*)this, pitch);
    }

private:

    float pitch = 1.0f;

};