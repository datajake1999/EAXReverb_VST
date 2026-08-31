#ifndef EAXREVERB_H
#define EAXREVERB_H

#include "public.sdk/source/vst2.x/audioeffectx.h"
#include "ReverbEffect.h"

enum
{
	// Global
	kNumPrograms = 113,

	// Parameters Tags
	kDisable = 0,
	kDgain,
	kDensity, kDiffusion, kGain, kGainHF, kGainLF, kDecayTime, kDecayHFRatio, kDecayLFRatio, kReflectionsGain, kReflectionsDelay, kReflectionsPanX, kReflectionsPanY, kReflectionsPanZ, kLateReverbGain, kLateReverbDelay, kLateReverbPanX, kLateReverbPanY, kLateReverbPanZ, kEchoTime, kEchoDepth, kModulationTime, kModulationDepth, kAirAbsorptionGainHF, kHFReference, kLFReference, kRoomRolloffFactor, kDecayHFLimit,

	kNumParams
};

typedef struct
{
	EFXEAXREVERBPROPERTIES properties;
	char name[32];
}EAXReverbProgram;

class EAXReverb : public AudioEffectX
{
public:
	EAXReverb (audioMasterCallback audioMaster);
	~EAXReverb ();
	virtual void setParameter (VstInt32 index, float value);
	virtual float getParameter (VstInt32 index);
	virtual void getParameterDisplay (VstInt32 index, char* text);
	virtual void getParameterLabel (VstInt32 index, char* label);
	virtual void getParameterName (VstInt32 index, char* text);
	virtual void setProgram (VstInt32 program);
	virtual void setProgramName (char* name);
	virtual void getProgramName (char* name);
	virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);
	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual VstInt32 getVendorVersion ();
	virtual VstPlugCategory getPlugCategory ();
	virtual void setSampleRate (float sampleRate);
	virtual void setBlockSize (VstInt32 blockSize);
	virtual void resume ();
	virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
private:
	void UpdateEffect();
	void SetReverbPreset(int preset);
	char *GetPresetName(int preset);
	ReverbEffect effect;
	float *floatSamplesIn;
	float floatSamplesOut[REVERB_BUFFERSIZE * OUTPUT_CHANNELS];
	float DisableEffect;
	float DryGain;
	EAXReverbProgram programs[kNumPrograms];
};

#endif
