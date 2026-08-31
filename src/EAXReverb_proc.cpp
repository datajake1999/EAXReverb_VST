#include "EAXReverb.h"

void EAXReverb::UpdateEffect()
{
	ReverbEffectLoadPreset(&effect, &programs[curProgram].properties);
	ReverbEffectUpdate(&effect, (int)sampleRate);
}

void EAXReverb::setSampleRate (float sampleRate)
{
	AudioEffectX::setSampleRate (sampleRate);
	ReverbEffectDestroy(&effect);
	ReverbEffectCreate(&effect, (int)sampleRate);
	UpdateEffect();
}

void EAXReverb::setBlockSize (VstInt32 blockSize)
{
	if (floatSamplesIn)
	{
		delete[] floatSamplesIn;
		floatSamplesIn = NULL;
	}
	AudioEffectX::setBlockSize (blockSize);
	floatSamplesIn = new float[blockSize];
	if (floatSamplesIn)
	{
		memset(floatSamplesIn, 0, blockSize*sizeof(float));
	}
}

void EAXReverb::resume ()
{
	AudioEffectX::resume ();
	UpdateEffect();
}

void EAXReverb::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	float* in1 = inputs[0];
	float* in2 = inputs[1];
	float* out1 = outputs[0];
	float* out2 = outputs[1];
	VstInt32 i;
	if (DisableEffect >= 0.5 || !floatSamplesIn)
	{
		if (out1)
		{
			if (in1)
			{
				memcpy(out1, in1, sampleFrames*sizeof(float));
			}
			else
			{
				memset(out1, 0, sampleFrames*sizeof(float));
			}
		}
		if (out2)
		{
			if (in2)
			{
				memcpy(out2, in2, sampleFrames*sizeof(float));
			}
			else
			{
				memset(out2, 0, sampleFrames*sizeof(float));
			}
		}
		return;
	}
	if (sampleFrames > blockSize)
	{
		sampleFrames = blockSize;
	}
	for (i = 0; i < sampleFrames; i++)
	{
		floatSamplesIn[i] = (in1[i] + in2[i]) / 2;
	}
	VstInt32 offset = 0;
	while (sampleFrames > 0)
	{
		VstInt32 currentSamples = REVERB_BUFFERSIZE / 4;
		if (currentSamples>sampleFrames)
		{
			currentSamples = sampleFrames;
		}
		ReverbEffectProcess(&effect, currentSamples, &floatSamplesIn[offset], floatSamplesOut);
		for (i = 0; i < currentSamples; i++)
		{
			in1[i+offset] = in1[i+offset] * DryGain;
			in2[i+offset] = in2[i+offset] * DryGain;
			out1[i+offset] = in1[i+offset] + floatSamplesOut[i*2+0];
			out2[i+offset] = in2[i+offset] + floatSamplesOut[i*2+1];
		}
		sampleFrames -= currentSamples;
		offset += currentSamples;
	}
}
