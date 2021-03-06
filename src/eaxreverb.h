
#ifndef __EAXReverb__
#define __EAXReverb__

#include "public.sdk/source/vst2.x/audioeffectx.h"
#include <set>
#include <string>
#include "biquad.h"
#include "biquadDouble.h"
#include "bitcrush.h"
#include "LinearResampler.h"
#include "resampler.h"
#include "ReverbEffect.h"
#include "ReverbEffectDouble.h"
#include "silence.h"
#include "ZOHResampler.h"

enum
{
	// Global
	kNumPrograms = 113,

	// Parameters Tags
	kDisable = 0,
	kMute,
	kInvertmode,
	kMonomode,
	kInvertorig,
	kInvertrev,
	kInvert,
	kSwaporig,
	kSwaprev,
	kSwap,
	kBalancelorig,
	kBalancerorig,
	kBalancelrev,
	kBalancerrev,
	kBalancel,
	kBalancer,
	kStereoorig,
	kStereorev,
	kStereo,
	kMonoorig,
	kMonorev,
	kMono,
	kOnlyorig,
	kOnlyrev,
	kMix,
	kDgain,
	kWgain,
	kMgain,
	kResample,
	kRsmq,
	kRsmmode,
	kRsmrate,
	kBitcrush,
	kBitdepth,
	kDither,
	kAutodither,
	kOnlyqerror,
	kLimit,
	kBqfilter,
	kFlttype,
	kFltfreq,
	kFltres,
	kFltgain,
	kIncorrect,
	kRate,
	kPreset,
	kDensity, kDiffusion, kGain, kGainHF, kGainLF, kDecayTime, kDecayHFRatio, kDecayLFRatio, kReflectionsGain, kReflectionsDelay, kReflectionsPanX, kReflectionsPanY, kReflectionsPanZ, kLateReverbGain, kLateReverbDelay, kLateReverbPanX, kLateReverbPanY, kLateReverbPanZ, kEchoTime, kEchoDepth, kModulationTime, kModulationDepth, kAirAbsorptionGainHF, kHFReference, kLFReference, kRoomRolloffFactor, kDecayHFLimit,

	kNumParams
};

class EAXReverb;


class EAXReverbProgram
{
	friend class EAXReverb;
public:
	EAXReverbProgram ();
	~EAXReverbProgram () {}

private:	
	float DisableEffect;
	float MuteEffect;
	float InvertMode;
	float MonoMode;
	float InvertOriginal;
	float InvertReverb;
	float Invert;
	float SwapOriginal;
	float SwapReverb;
	float Swap;
	float BalanceLOriginal;
	float BalanceROriginal;
	float BalanceLReverb;
	float BalanceRReverb;
	float BalanceL;
	float BalanceR;
	float StereoWidthOriginal;
	float StereoWidthReverb;
	float StereoWidth;
	float MonoOriginal;
	float MonoReverb;
	float Mono;
	float OnlyOriginal;
	float OnlyReverb;
	float MixMode;
	float DryGain;
	float WetGain;
	float MasterGain;
	float Resample;
	float RSMQ;
	float RSMMode;
	float RSMRate;
	float BitCr;
	float BitDepth;
	float Dither;
	float AutoDither;
	float OnlyQError;
	float Limit;
	float BQFilter;
	float FLTType;
	float FLTFreq;
	float FLTRes;
	float FLTGain;
	float IncorrectMode;
	float ReverbRate;
	float ReverbPreset;
	float Density; float Diffusion; float Gain; float GainHF; float GainLF; float DecayTime; float DecayHFRatio; float DecayLFRatio; float ReflectionsGain; float ReflectionsDelay; float ReflectionsPanX; float ReflectionsPanY; float ReflectionsPanZ; float LateReverbGain; float LateReverbDelay; float LateReverbPanX; float LateReverbPanY; float LateReverbPanZ; float EchoTime; float EchoDepth; float ModulationTime; float ModulationDepth; float AirAbsorptionGainHF; float HFReference; float LFReference; float RoomRolloffFactor; float DecayHFLimit;
	char name[32];
};


class EAXReverb : public AudioEffectX
{
public:
	EAXReverb (audioMasterCallback audioMaster);
	~EAXReverb ();

	virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
	virtual void processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames);

	virtual VstInt32 getProgram ();
	virtual void setProgram (VstInt32 program);
	virtual void setProgramName (char* name);
	virtual void getProgramName (char* name);
	virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);
	
	virtual void resume ();
	
	virtual VstInt32 setChunk (void* data, VstInt32 byteSize, bool isPreset);
	virtual VstInt32 getChunk (void** data, bool isPreset);
	virtual void setParameter (VstInt32 index, float value);
	virtual void setParameterAutomated (VstInt32 index, float value);
	virtual float getParameter (VstInt32 index);
	virtual void getParameterDisplay (VstInt32 index, char* text);
	virtual void getParameterLabel (VstInt32 index, char* label);
	virtual void getParameterName (VstInt32 index, char* text);
	virtual VstInt32 canDo(char *text);

	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual VstInt32 getVendorVersion () { return 1000; }
	
	virtual VstPlugCategory getPlugCategory () { return kPlugCategEffect; }

private:
	void SetDisableEffect(float val);
	void SetMuteEffect(float val);
	void SetInvertMode(float val);
	void SetMonoMode(float val);
	void SetInvertOriginal(float val);
	void SetInvertReverb(float val);
	void SetInvert(float val);
	void SetSwapOriginal(float val);
	void SetSwapReverb(float val);
	void SetSwap(float val);
	void SetBalanceLOriginal(float val);
	void SetBalanceROriginal(float val);
	void SetBalanceLReverb(float val);
	void SetBalanceRReverb(float val);
	void SetBalanceL(float val);
	void SetBalanceR(float val);
	void SetStereoWidthOriginal(float val);
	void SetStereoWidthReverb(float val);
	void SetStereoWidth(float val);
	void SetMonoOriginal(float val);
	void SetMonoReverb(float val);
	void SetMono(float val);
	void SetOnlyOriginal(float val);
	void SetOnlyReverb(float val);
	void SetMixMode(float val);
	void SetDryGain(float val);
	void SetWetGain(float val);
	void SetMasterGain(float val);
	void SetResample(float val);
	void SetRSMQ(float val);
	void SetRSMMode(float val);
	void SetRSMRate(float val);
	void SetBitCrush(float val);
	void SetBitDepth(float val);
	void SetDither(float val);
	void SetAutoD(float val);
	void SetOnlyQError(float val);
	void SetLimit(float val);
	void SetBQFilter(float val);
	void SetFLTType(float val);
	void SetFLTFreq(float val);
	void SetFLTRes(float val);
	void SetFLTGain(float val);
	void SetIncorrectMode(float val);
	void SetReverbRate(float val);
	void SetReverbPreset(int preset, bool update);
	char *GetPresetName(int preset);
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

	bitcrusher bc;
	ReverbEffect effect;
	ReverbEffectDouble effectDouble;
	sf_biquad_state_st bq_state;
	sfd_biquad_state_st bq_state_double;
	void *linearresampler1;
	void *linearresampler2;
	void *resampler1;
	void *resampler2;
	void *zohresampler1;
	void *zohresampler2;
	EAXReverbProgram* programs;
	
	float DisableEffect;
	float MuteEffect;
	float InvertMode;
	float MonoMode;
	float InvertOriginal;
	float InvertReverb;
	float Invert;
	float SwapOriginal;
	float SwapReverb;
	float Swap;
	float BalanceLOriginal;
	float BalanceROriginal;
	float BalanceLReverb;
	float BalanceRReverb;
	float BalanceL;
	float BalanceR;
	float StereoWidthOriginal;
	float StereoWidthReverb;
	float StereoWidth;
	float MonoOriginal;
	float MonoReverb;
	float Mono;
	float OnlyOriginal;
	float OnlyReverb;
	float MixMode;
	float DryGain;
	float WetGain;
	float MasterGain;
	float Resample;
	float RSMQ;
	float RSMMode;
	float RSMRate;
	float BitCr;
	float BitDepth;
	float Dither;
	float AutoDither;
	float OnlyQError;
	float Limit;
	float BQFilter;
	float FLTType;
	float FLTFreq;
	float FLTRes;
	float FLTGain;
	float IncorrectMode;
	float ReverbRate;
	float ReverbPreset;
	int i_ReverbPreset;
	float Density; float Diffusion; float Gain; float GainHF; float GainLF; float DecayTime; float DecayHFRatio; float DecayLFRatio; float ReflectionsGain; float ReflectionsDelay; float ReflectionsPanX; float ReflectionsPanY; float ReflectionsPanZ; float LateReverbGain; float LateReverbDelay; float LateReverbPanX; float LateReverbPanY; float LateReverbPanZ; float EchoTime; float EchoDepth; float ModulationTime; float ModulationDepth; float AirAbsorptionGainHF; float HFReference; float LFReference; float RoomRolloffFactor; float DecayHFLimit;
	int i_DecayHFLimit;
	
	int rate;
	int rate_new;
	int rsm;
	int bits;
	std::set< std::string > _canDo;
};

#endif
