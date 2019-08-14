
#ifndef __eaxreverb__
#define __eaxreverb__

#include "public.sdk/source/vst2.x/audioeffectx.h"
#include "ReverbEffect.h"

enum
{
	// Global
	kNumPrograms = 113,

	// Parameters Tags
	kInvertrev = 0,
	kMonorev,
	kOnlyorig,
	kOnlyrev,
	kDgain,
	kWgain,
	kPreset,
	kDensity, kDiffusion, kGain, kGainHF, kGainLF, kDecayTime, kDecayHFRatio, kDecayLFRatio, kReflectionsGain, kReflectionsDelay, kReflectionsPanX, kReflectionsPanY, kReflectionsPanZ, kLateReverbGain, kLateReverbDelay, kLateReverbPanX, kLateReverbPanY, kLateReverbPanZ, kEchoTime, kEchoDepth, kModulationTime, kModulationDepth, kAirAbsorptionGainHF, kHFReference, kLFReference, kRoomRolloffFactor, kDecayHFLimit,

	kNumParams
};

class eaxreverb;


class eaxreverbProgram
{
	friend class eaxreverb;
public:
	eaxreverbProgram ();
	~eaxreverbProgram () {}

private:	
	char name[24];
};


class eaxreverb : public AudioEffectX
{
public:
	eaxreverb (audioMasterCallback audioMaster);
	~eaxreverb ();

	virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);

	virtual VstInt32 getProgram ();
	virtual void setProgram (VstInt32 program);
	virtual void setProgramName (char* name);
	virtual void getProgramName (char* name);
	virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);
	
	virtual void setParameter (VstInt32 index, float value);
	virtual void setParameterAutomated (VstInt32 index, float value);
	virtual float getParameter (VstInt32 index);
	virtual void getParameterDisplay (VstInt32 index, char* text);
	virtual void getParameterLabel (VstInt32 index, char* label);
	virtual void getParameterName (VstInt32 index, char* text);

	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual VstInt32 getVendorVersion () { return 1000; }
	
	virtual VstPlugCategory getPlugCategory () { return kPlugCategEffect; }

private:
	void SetReverbPreset(int preset);
	char *GetPresetName(int preset);
	void SetInvertReverb(float val);
	void SetMonoReverb(float val);
	void SetOnlyOriginal(float val);
	void SetOnlyReverb(float val);
	void SetDryGain(float val);
	void SetWetGain(float val);
	void SetDensity(float val);
	void SetDiffusion(float val);
	void SetGain(float val);
	void SetGainHF(float val);
	void SetGainLF(float val);
	void SetDecayTime(float val);
	void SetDecayHFRatio(float val);
	void SetDecayLFRatio(float val);
	void SetReflectionsGain(float val);
	void SetReflectionsDelay(float val);
	void SetReflectionsPanX(float val);
	void SetReflectionsPanY(float val);
	void SetReflectionsPanZ(float val);
	void SetLateReverbGain(float val);
	void SetLateReverbDelay(float val);
	void SetLateReverbPanX(float val);
	void SetLateReverbPanY(float val);
	void SetLateReverbPanZ(float val);
	void SetEchoTime(float val);
	void SetEchoDepth(float val);
	void SetModulationTime(float val);
	void SetModulationDepth(float val);
	void SetAirAbsorptionGainHF(float val);
	void SetHFReference(float val);
	void SetLFReference(float val);
	void SetRoomRolloffFactor(float val);
	void SetDecayHFLimit(float val);
	void SetEAXParams(float Density, float Diffusion, float Gain, float GainHF, float GainLF, float DecayTime, float DecayHFRatio, float DecayLFRatio, float ReflectionsGain, float ReflectionsDelay, float ReflectionsPanX, float ReflectionsPanY, float ReflectionsPanZ, float LateReverbGain, float LateReverbDelay, float LateReverbPanX, float LateReverbPanY, float LateReverbPanZ, float EchoTime, float EchoDepth, float ModulationTime, float ModulationDepth, float AirAbsorptionGainHF, float HFReference, float LFReference, float RoomRolloffFactor, float DecayHFLimit);

	ReverbEffect effect;
	eaxreverbProgram* programs;
	
	float ReverbPreset;
	float InvertReverb;
	float MonoReverb;
	float OnlyReverb;
	float OnlyOriginal;
	float DryGain;
	float WetGain;
	float Density; float Diffusion; float Gain; float GainHF; float GainLF; float DecayTime; float DecayHFRatio; float DecayLFRatio; float ReflectionsGain; float ReflectionsDelay; float ReflectionsPanX; float ReflectionsPanY; float ReflectionsPanZ; float LateReverbGain; float LateReverbDelay; float LateReverbPanX; float LateReverbPanY; float LateReverbPanZ; float EchoTime; float EchoDepth; float ModulationTime; float ModulationDepth; float AirAbsorptionGainHF; float HFReference; float LFReference; float RoomRolloffFactor; float DecayHFLimit;
	int i_DecayHFLimit;
	
	int rate;
};

#endif
