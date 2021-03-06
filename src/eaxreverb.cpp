
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __EAXReverb__
#include "EAXReverb.h"
#endif


EAXReverbProgram::EAXReverbProgram ()
{
	// default Program Values
	DisableEffect = 0;
	MuteEffect = 0;
	InvertMode = 0;
	MonoMode = 0;
	InvertOriginal = 0;
	InvertReverb = 0;
	Invert = 0;
	SwapOriginal = 0;
	SwapReverb = 0;
	Swap = 0;
	BalanceLOriginal = 1;
	BalanceROriginal = 1;
	BalanceLReverb = 1;
	BalanceRReverb = 1;
	BalanceL = 1;
	BalanceR = 1;
	StereoWidthOriginal = 1;
	StereoWidthReverb = 1;
	StereoWidth = 1;
	MonoOriginal = 0;
	MonoReverb = 0;
	Mono = 0;
	OnlyOriginal = 0;
	OnlyReverb = 0;
	MixMode = 0;
	DryGain = 1;
	WetGain = 1;
	MasterGain = 1;
	Resample = 0;
	RSMQ = 0;
	RSMMode = 0;
	RSMRate = 1;
	BitCr = 0;
	BitDepth = 8;
	Dither = 0;
	AutoDither = 0;
	OnlyQError = 0;
	Limit = 1;
	BQFilter = 0;
	FLTType = 0;
	FLTFreq = 440;
	FLTRes = 0;
	FLTGain = 0;
	IncorrectMode = 0;
	ReverbRate = 44100;
}


EAXReverb::EAXReverb (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, kNumPrograms, kNumParams)
{
	// init
	resume ();
	
	programs = new EAXReverbProgram[numPrograms];

	if (programs)
	for (int i = 0; i < numPrograms; i++)
	{
		setProgram (i);
		SetReverbPreset (i, false);
		setProgramName (GetPresetName(i));
	}
	setProgram (0);

	setNumInputs (2);	// stereo input
	setNumOutputs (2);	// stereo output
	setUniqueID ('EAX');
	_canDo.insert("plugAsChannelInsert"); // plug-in can be used as a channel insert effect.
	_canDo.insert("plugAsSend"); // plug-in can be used as a send effect.
	_canDo.insert("x2in2out"); 
	canProcessReplacing ();
	canDoubleReplacing ();
	programsAreChunks(true);

}


EAXReverb::~EAXReverb ()
{
	effect.Destroy();
	effectDouble.Destroy();
	LinearResamplerReset(linearresampler1);
	LinearResamplerDestroy(linearresampler1);
	LinearResamplerReset(linearresampler2);
	LinearResamplerDestroy(linearresampler2);
	resampler_clear(resampler1);
	resampler_destroy(resampler1);
	resampler_clear(resampler2);
	resampler_destroy(resampler2);
	ZOHResamplerReset(zohresampler1);
	ZOHResamplerDestroy(zohresampler1);
	ZOHResamplerReset(zohresampler2);
	ZOHResamplerDestroy(zohresampler2);
	if (programs)
	delete[] programs;
}


VstInt32 EAXReverb::getProgram ()
{
	return curProgram;
}


void EAXReverb::setProgram (VstInt32 program)
{
	EAXReverbProgram* ap = &programs[program];

	curProgram = program;
	setParameter (kDisable, ap->DisableEffect);	
	setParameter (kMute, ap->MuteEffect);	
	setParameter (kInvertmode, ap->InvertMode);	
	setParameter (kMonomode, ap->MonoMode);	
	setParameter (kInvertorig, ap->InvertOriginal);	
	setParameter (kInvertrev, ap->InvertReverb);	
	setParameter (kInvert, ap->Invert);	
	setParameter (kSwaporig, ap->SwapOriginal);	
	setParameter (kSwaprev, ap->SwapReverb);	
	setParameter (kSwap, ap->Swap);	
	setParameter (kBalancelorig, ap->BalanceLOriginal);	
	setParameter (kBalancerorig, ap->BalanceROriginal);	
	setParameter (kBalancelrev, ap->BalanceLReverb);	
	setParameter (kBalancerrev, ap->BalanceRReverb);	
	setParameter (kBalancel, ap->BalanceL);	
	setParameter (kBalancer, ap->BalanceR);	
	setParameter (kStereoorig, ap->StereoWidthOriginal);	
	setParameter (kStereorev, ap->StereoWidthReverb);	
	setParameter (kStereo, ap->StereoWidth);	
	setParameter (kMonoorig, ap->MonoOriginal);	
	setParameter (kMonorev, ap->MonoReverb);	
	setParameter (kMono, ap->Mono);	
	setParameter (kOnlyorig, ap->OnlyOriginal);	
	setParameter (kOnlyrev, ap->OnlyReverb);	
	setParameter (kMix, ap->MixMode);	
	setParameter (kDgain, ap->DryGain);	
	setParameter (kWgain, ap->WetGain);	
	setParameter (kMgain, ap->MasterGain);	
	setParameter (kResample, ap->Resample);	
	setParameter (kRsmq, ap->RSMQ);	
	setParameter (kRsmmode, ap->RSMMode);	
	setParameter (kRsmrate, ap->RSMRate/64);	
	setParameter (kBitcrush, ap->BitCr);	
	setParameter (kBitdepth, ap->BitDepth/16);	
	setParameter (kDither, ap->Dither);	
	setParameter (kAutodither, ap->AutoDither);	
	setParameter (kOnlyqerror, ap->OnlyQError);	
	setParameter (kLimit, ap->Limit);	
	setParameter (kBqfilter, ap->BQFilter);	
	setParameter (kFlttype, ap->FLTType);	
	setParameter (kFltfreq, ap->FLTFreq/20000);	
	setParameter (kFltres, ap->FLTRes/10);	
	setParameter (kFltgain, ap->FLTGain/10);	
	setParameter (kIncorrect, ap->IncorrectMode);	
	setParameter (kRate, ap->ReverbRate/1000000);	
	ReverbPreset = ap->ReverbPreset;
	programs[curProgram].ReverbPreset = ap->ReverbPreset;
	i_ReverbPreset = int(ap->ReverbPreset);
	setParameter (kDensity, ap->Density/EAXREVERB_MAX_DENSITY);	
	setParameter (kDiffusion, ap->Diffusion/EAXREVERB_MAX_DIFFUSION);	
	setParameter (kGain, ap->Gain/EAXREVERB_MAX_GAIN);	
	setParameter (kGainHF, ap->GainHF/EAXREVERB_MAX_GAINHF);	
	setParameter (kGainLF, ap->GainLF/EAXREVERB_MAX_GAINLF);	
	setParameter (kDecayTime, ap->DecayTime/EAXREVERB_MAX_DECAY_TIME);	
	setParameter (kDecayHFRatio, ap->DecayHFRatio/EAXREVERB_MAX_DECAY_HFRATIO);	
	setParameter (kDecayLFRatio, ap->DecayLFRatio/EAXREVERB_MAX_DECAY_LFRATIO);	
	setParameter (kReflectionsGain, ap->ReflectionsGain/EAXREVERB_MAX_REFLECTIONS_GAIN);	
	setParameter (kReflectionsDelay, ap->ReflectionsDelay/EAXREVERB_MAX_REFLECTIONS_DELAY);	
	setParameter (kReflectionsPanX, ap->ReflectionsPanX);	
	setParameter (kReflectionsPanY, ap->ReflectionsPanY);	
	setParameter (kReflectionsPanZ, ap->ReflectionsPanZ);	
	setParameter (kLateReverbGain, ap->LateReverbGain/EAXREVERB_MAX_LATE_REVERB_GAIN);	
	setParameter (kLateReverbDelay, ap->LateReverbDelay/EAXREVERB_MAX_LATE_REVERB_DELAY);	
	setParameter (kLateReverbPanX, ap->LateReverbPanX);	
	setParameter (kLateReverbPanY, ap->LateReverbPanY);	
	setParameter (kLateReverbPanZ, ap->LateReverbPanZ);	
	setParameter (kEchoTime, ap->EchoTime/EAXREVERB_MAX_ECHO_TIME);	
	setParameter (kEchoDepth, ap->EchoDepth/EAXREVERB_MAX_ECHO_DEPTH);	
	setParameter (kModulationTime, ap->ModulationTime/EAXREVERB_MAX_MODULATION_TIME);	
	setParameter (kModulationDepth, ap->ModulationDepth/EAXREVERB_MAX_MODULATION_DEPTH);	
	setParameter (kAirAbsorptionGainHF, ap->AirAbsorptionGainHF/EAXREVERB_MAX_AIR_ABSORPTION_GAINHF);	
	setParameter (kHFReference, ap->HFReference/EAXREVERB_MAX_HFREFERENCE);	
	setParameter (kLFReference, ap->LFReference/EAXREVERB_MAX_LFREFERENCE);	
	setParameter (kRoomRolloffFactor, ap->RoomRolloffFactor/EAXREVERB_MAX_ROOM_ROLLOFF_FACTOR);	
	setParameter (kDecayHFLimit, ap->DecayHFLimit);	
}


void EAXReverb::SetDisableEffect (float val)
{
	DisableEffect = val;
	programs[curProgram].DisableEffect = val;
}


void EAXReverb::SetMuteEffect (float val)
{
	MuteEffect = val;
	programs[curProgram].MuteEffect = val;
}


void EAXReverb::SetInvertMode (float val)
{
	InvertMode = val;
	programs[curProgram].InvertMode = val;
}


void EAXReverb::SetMonoMode (float val)
{
	MonoMode = val;
	programs[curProgram].MonoMode = val;
}


void EAXReverb::SetInvertOriginal (float val)
{
	InvertOriginal = val;
	programs[curProgram].InvertOriginal = val;
}


void EAXReverb::SetInvertReverb (float val)
{
	InvertReverb = val;
	programs[curProgram].InvertReverb = val;
}


void EAXReverb::SetInvert (float val)
{
	Invert = val;
	programs[curProgram].Invert = val;
}


void EAXReverb::SetSwapOriginal (float val)
{
	SwapOriginal = val;
	programs[curProgram].SwapOriginal = val;
}


void EAXReverb::SetSwapReverb (float val)
{
	SwapReverb = val;
	programs[curProgram].SwapReverb = val;
}


void EAXReverb::SetSwap (float val)
{
	Swap = val;
	programs[curProgram].Swap = val;
}


void EAXReverb::SetBalanceLOriginal (float val)
{
	BalanceLOriginal = val;
	programs[curProgram].BalanceLOriginal = val;
}


void EAXReverb::SetBalanceROriginal (float val)
{
	BalanceROriginal = val;
	programs[curProgram].BalanceROriginal = val;
}


void EAXReverb::SetBalanceLReverb (float val)
{
	BalanceLReverb = val;
	programs[curProgram].BalanceLReverb = val;
}


void EAXReverb::SetBalanceRReverb (float val)
{
	BalanceRReverb = val;
	programs[curProgram].BalanceRReverb = val;
}


void EAXReverb::SetBalanceL (float val)
{
	BalanceL = val;
	programs[curProgram].BalanceL = val;
}


void EAXReverb::SetBalanceR (float val)
{
	BalanceR = val;
	programs[curProgram].BalanceR = val;
}


void EAXReverb::SetStereoWidthOriginal (float val)
{
	StereoWidthOriginal = val;
	programs[curProgram].StereoWidthOriginal = val;
}


void EAXReverb::SetStereoWidthReverb (float val)
{
	StereoWidthReverb = val;
	programs[curProgram].StereoWidthReverb = val;
}


void EAXReverb::SetStereoWidth (float val)
{
	StereoWidth = val;
	programs[curProgram].StereoWidth = val;
}


void EAXReverb::SetMonoOriginal (float val)
{
	MonoOriginal = val;
	programs[curProgram].MonoOriginal = val;
}


void EAXReverb::SetMonoReverb (float val)
{
	MonoReverb = val;
	programs[curProgram].MonoReverb = val;
}


void EAXReverb::SetMono (float val)
{
	Mono = val;
	programs[curProgram].Mono = val;
}


void EAXReverb::SetOnlyOriginal (float val)
{
	OnlyOriginal = val;
	programs[curProgram].OnlyOriginal = val;
}


void EAXReverb::SetOnlyReverb (float val)
{
	OnlyReverb = val;
	programs[curProgram].OnlyReverb = val;
}


void EAXReverb::SetMixMode (float val)
{
	MixMode = val;
	programs[curProgram].MixMode = val;
}


void EAXReverb::SetDryGain (float val)
{
	DryGain = val;
	programs[curProgram].DryGain = val;
}


void EAXReverb::SetWetGain (float val)
{
	WetGain = val;
	programs[curProgram].WetGain = val;
}


void EAXReverb::SetMasterGain (float val)
{
	MasterGain = val;
	programs[curProgram].MasterGain = val;
}


void EAXReverb::SetResample (float val)
{
	Resample = val;
	programs[curProgram].Resample = val;
}


void EAXReverb::SetRSMQ (float val)
{
	RSMQ = val;
	programs[curProgram].RSMQ = val;
}


void EAXReverb::SetRSMMode (float val)
{
	RSMMode = val;
	programs[curProgram].RSMMode = val;
	if (RSMRate >= 1.0)
	{
		rsm = int(RSMRate);
	}
	else
	{
		rsm = 1;
	}
	if (RSMMode >= 0.5)
	{
		rate_new = rate*rsm;
	}
	else
	{
		rate_new = rate/rsm;
	}
	LinearResamplerSetup(linearresampler1, rate, rate_new);
	LinearResamplerSetup(linearresampler2, rate_new, rate);
	resampler_set_rate(resampler1, (double)rate / (double)rate_new);
	resampler_set_rate(resampler2, (double)rate_new / (double)rate);
	ZOHResamplerSetup(zohresampler1, rate, rate_new);
	ZOHResamplerSetup(zohresampler2, rate_new, rate);
}


void EAXReverb::SetRSMRate (float val)
{
	if (val > 64)
	{
		val = 64;
	}
	else if (val < 1)
	{
		val = 1;
	}
	RSMRate = val;
	programs[curProgram].RSMRate = val;
	if (RSMRate >= 1.0)
	{
		rsm = int(RSMRate);
	}
	else
	{
		rsm = 1;
	}
	if (RSMMode >= 0.5)
	{
		rate_new = rate*rsm;
	}
	else
	{
		rate_new = rate/rsm;
	}
	LinearResamplerSetup(linearresampler1, rate, rate_new);
	LinearResamplerSetup(linearresampler2, rate_new, rate);
	resampler_set_rate(resampler1, (double)rate / (double)rate_new);
	resampler_set_rate(resampler2, (double)rate_new / (double)rate);
	ZOHResamplerSetup(zohresampler1, rate, rate_new);
	ZOHResamplerSetup(zohresampler2, rate_new, rate);
}


void EAXReverb::SetBitCrush (float val)
{
	BitCr = val;
	programs[curProgram].BitCr = val;
}


void EAXReverb::SetBitDepth (float val)
{
	if (val > 16)
	{
		val = 16;
	}
	else if (val < 1)
	{
		val = 1;
	}
	BitDepth = val;
	programs[curProgram].BitDepth = val;
	bits = int(BitDepth);
	SetCrushAmount(&bc, bits);
}


void EAXReverb::SetDither (float val)
{
	Dither = val;
	programs[curProgram].Dither = val;
}


void EAXReverb::SetAutoD (float val)
{
	AutoDither = val;
	programs[curProgram].AutoDither = val;
	if (AutoDither >= 0.5)
	{
		SetAutoDither(&bc, 1);
	}
	else
	{
		SetAutoDither(&bc, 0);
	}
}


void EAXReverb::SetOnlyQError (float val)
{
	OnlyQError = val;
	programs[curProgram].OnlyQError = val;
	if (OnlyQError >= 0.5)
	{
		SetOnlyError(&bc, 1);
	}
	else
	{
		SetOnlyError(&bc, 0);
	}
}


void EAXReverb::SetLimit (float val)
{
	Limit = val;
	programs[curProgram].Limit = val;
}


void EAXReverb::SetBQFilter (float val)
{
	BQFilter = val;
	programs[curProgram].BQFilter = val;
}


void EAXReverb::SetFLTType (float val)
{
	FLTType = val;
	programs[curProgram].FLTType = val;
}


void EAXReverb::SetFLTFreq (float val)
{
	if (val > 20000)
	{
		val = 20000;
	}
	else if (val < 20)
	{
		val = 20;
	}
	if (val > rate/2-2)
	{
		val = float(rate/2-2);
	}
	FLTFreq = val;
	programs[curProgram].FLTFreq = val;
}


void EAXReverb::SetFLTRes (float val)
{
	if (val > 10)
	{
		val = 10;
	}
	else if (val < 0)
	{
		val = 0;
	}
	FLTRes = val;
	programs[curProgram].FLTRes = val;
}


void EAXReverb::SetFLTGain (float val)
{
	if (val > 10)
	{
		val = 10;
	}
	else if (val < 0)
	{
		val = 0;
	}
	FLTGain = val;
	programs[curProgram].FLTGain = val;
}


void EAXReverb::SetIncorrectMode (float val)
{
	IncorrectMode = val;
	programs[curProgram].IncorrectMode = val;
	effect.LoadPreset(Density, Diffusion, Gain, GainHF, GainLF, DecayTime, DecayHFRatio, DecayLFRatio, ReflectionsGain, ReflectionsDelay, ReflectionsPanX, ReflectionsPanY, ReflectionsPanZ, LateReverbGain, LateReverbDelay, LateReverbPanX, LateReverbPanY, LateReverbPanZ, EchoTime, EchoDepth, ModulationTime, ModulationDepth, AirAbsorptionGainHF, HFReference, LFReference, RoomRolloffFactor, i_DecayHFLimit);
	effectDouble.LoadPreset(Density, Diffusion, Gain, GainHF, GainLF, DecayTime, DecayHFRatio, DecayLFRatio, ReflectionsGain, ReflectionsDelay, ReflectionsPanX, ReflectionsPanY, ReflectionsPanZ, LateReverbGain, LateReverbDelay, LateReverbPanX, LateReverbPanY, LateReverbPanZ, EchoTime, EchoDepth, ModulationTime, ModulationDepth, AirAbsorptionGainHF, HFReference, LFReference, RoomRolloffFactor, i_DecayHFLimit);
	if (IncorrectMode >= 0.5)	
	{
		effect.Update(int(ReverbRate));
		effectDouble.Update(int(ReverbRate));
	}
	else
	{
		effect.Update(rate);
		effectDouble.Update(rate);
	}
}


void EAXReverb::SetReverbRate (float val)
{
	if (val > 1000000)
	{
		val = 1000000;
	}
	else if (val < 1000)
	{
		val = 1000;
	}
	ReverbRate = val;
	programs[curProgram].ReverbRate = val;
	if (IncorrectMode >= 0.5)	
	{
		effect.LoadPreset(Density, Diffusion, Gain, GainHF, GainLF, DecayTime, DecayHFRatio, DecayLFRatio, ReflectionsGain, ReflectionsDelay, ReflectionsPanX, ReflectionsPanY, ReflectionsPanZ, LateReverbGain, LateReverbDelay, LateReverbPanX, LateReverbPanY, LateReverbPanZ, EchoTime, EchoDepth, ModulationTime, ModulationDepth, AirAbsorptionGainHF, HFReference, LFReference, RoomRolloffFactor, i_DecayHFLimit);
		effectDouble.LoadPreset(Density, Diffusion, Gain, GainHF, GainLF, DecayTime, DecayHFRatio, DecayLFRatio, ReflectionsGain, ReflectionsDelay, ReflectionsPanX, ReflectionsPanY, ReflectionsPanZ, LateReverbGain, LateReverbDelay, LateReverbPanX, LateReverbPanY, LateReverbPanZ, EchoTime, EchoDepth, ModulationTime, ModulationDepth, AirAbsorptionGainHF, HFReference, LFReference, RoomRolloffFactor, i_DecayHFLimit);
		effect.Update(int(ReverbRate));
		effectDouble.Update(int(ReverbRate));
	}
}


void EAXReverb::SetReverbPreset(int preset, bool update) {
	ReverbPreset = float(preset);
	programs[curProgram].ReverbPreset = float(preset);
	i_ReverbPreset = preset;
	//set a preset
	if (preset == 0)
	{
		SetEAXParams(EFX_REVERB_PRESET_GENERIC);
	}
	else if (preset == 1)
	{
		SetEAXParams(EFX_REVERB_PRESET_PADDEDCELL);
	}
	else if (preset == 2)
	{
		SetEAXParams(EFX_REVERB_PRESET_ROOM);
	}
	else if (preset == 3)
	{
		SetEAXParams(EFX_REVERB_PRESET_BATHROOM);
	}
	else if (preset == 4)
	{
		SetEAXParams(EFX_REVERB_PRESET_LIVINGROOM);
	}
	else if (preset == 5)
	{
		SetEAXParams(EFX_REVERB_PRESET_STONEROOM);
	}
	else if (preset == 6)
	{
		SetEAXParams(EFX_REVERB_PRESET_AUDITORIUM);
	}
	else if (preset == 7)
	{
		SetEAXParams(EFX_REVERB_PRESET_CONCERTHALL);
	}
	else if (preset == 8)
	{
		SetEAXParams(EFX_REVERB_PRESET_CAVE);
	}
	else if (preset == 9)
	{
		SetEAXParams(EFX_REVERB_PRESET_ARENA);
	}
	else if (preset == 10)
	{
		SetEAXParams(EFX_REVERB_PRESET_HANGAR);
	}
	else if (preset == 11)
	{
		SetEAXParams(EFX_REVERB_PRESET_CARPETEDHALLWAY);
	}
	else if (preset == 12)
	{
		SetEAXParams(EFX_REVERB_PRESET_HALLWAY);
	}
	else if (preset == 13)
	{
		SetEAXParams(EFX_REVERB_PRESET_STONECORRIDOR);
	}
	else if (preset == 14)
	{
		SetEAXParams(EFX_REVERB_PRESET_ALLEY);
	}
	else if (preset == 15)
	{
		SetEAXParams(EFX_REVERB_PRESET_FOREST);
	}
	else if (preset == 16)
	{
		SetEAXParams(EFX_REVERB_PRESET_CITY);
	}
	else if (preset == 17)
	{
		SetEAXParams(EFX_REVERB_PRESET_MOUNTAINS);
	}
	else if (preset == 18)
	{
		SetEAXParams(EFX_REVERB_PRESET_QUARRY);
	}
	else if (preset == 19)
	{
		SetEAXParams(EFX_REVERB_PRESET_PLAIN);
	}
	else if (preset == 20)
	{
		SetEAXParams(EFX_REVERB_PRESET_PARKINGLOT);
	}
	else if (preset == 21)
	{
		SetEAXParams(EFX_REVERB_PRESET_SEWERPIPE);
	}
	else if (preset == 22)
	{
		SetEAXParams(EFX_REVERB_PRESET_UNDERWATER);
	}
	else if (preset == 23)
	{
		SetEAXParams(EFX_REVERB_PRESET_DRUGGED);
	}
	else if (preset == 24)
	{
		SetEAXParams(EFX_REVERB_PRESET_DIZZY);
	}
	else if (preset == 25)
	{
		SetEAXParams(EFX_REVERB_PRESET_PSYCHOTIC);
	}
	else if (preset == 26)
	{
		SetEAXParams(EFX_REVERB_PRESET_CASTLE_SMALLROOM);
	}
	else if (preset == 27)
	{
		SetEAXParams(EFX_REVERB_PRESET_CASTLE_SHORTPASSAGE);
	}
	else if (preset == 28)
	{
		SetEAXParams(EFX_REVERB_PRESET_CASTLE_MEDIUMROOM);
	}
	else if (preset == 29)
	{
		SetEAXParams(EFX_REVERB_PRESET_CASTLE_LARGEROOM);
	}
	else if (preset == 30)
	{
		SetEAXParams(EFX_REVERB_PRESET_CASTLE_LONGPASSAGE);
	}
	else if (preset == 31)
	{
		SetEAXParams(EFX_REVERB_PRESET_CASTLE_HALL);
	}
	else if (preset == 32)
	{
		SetEAXParams(EFX_REVERB_PRESET_CASTLE_CUPBOARD);
	}
	else if (preset == 33)
	{
		SetEAXParams(EFX_REVERB_PRESET_CASTLE_COURTYARD);
	}
	else if (preset == 34)
	{
		SetEAXParams(EFX_REVERB_PRESET_CASTLE_ALCOVE);
	}
	else if (preset == 35)
	{
		SetEAXParams(EFX_REVERB_PRESET_FACTORY_SMALLROOM);
	}
	else if (preset == 36)
	{
		SetEAXParams(EFX_REVERB_PRESET_FACTORY_SHORTPASSAGE);
	}
	else if (preset == 37)
	{
		SetEAXParams(EFX_REVERB_PRESET_FACTORY_MEDIUMROOM);
	}
	else if (preset == 38)
	{
		SetEAXParams(EFX_REVERB_PRESET_FACTORY_LARGEROOM);
	}
	else if (preset == 39)
	{
		SetEAXParams(EFX_REVERB_PRESET_FACTORY_LONGPASSAGE);
	}
	else if (preset == 40)
	{
		SetEAXParams(EFX_REVERB_PRESET_FACTORY_HALL);
	}
	else if (preset == 41)
	{
		SetEAXParams(EFX_REVERB_PRESET_FACTORY_CUPBOARD);
	}
	else if (preset == 42)
	{
		SetEAXParams(EFX_REVERB_PRESET_FACTORY_COURTYARD);
	}
	else if (preset == 43)
	{
		SetEAXParams(EFX_REVERB_PRESET_FACTORY_ALCOVE);
	}
	else if (preset == 44)
	{
		SetEAXParams(EFX_REVERB_PRESET_ICEPALACE_SMALLROOM);
	}
	else if (preset == 45)
	{
		SetEAXParams(EFX_REVERB_PRESET_ICEPALACE_SHORTPASSAGE);
	}
	else if (preset == 46)
	{
		SetEAXParams(EFX_REVERB_PRESET_ICEPALACE_MEDIUMROOM);
	}
	else if (preset == 47)
	{
		SetEAXParams(EFX_REVERB_PRESET_ICEPALACE_LARGEROOM);
	}
	else if (preset == 48)
	{
		SetEAXParams(EFX_REVERB_PRESET_ICEPALACE_LONGPASSAGE);
	}
	else if (preset == 49)
	{
		SetEAXParams(EFX_REVERB_PRESET_ICEPALACE_HALL);
	}
	else if (preset == 50)
	{
		SetEAXParams(EFX_REVERB_PRESET_ICEPALACE_CUPBOARD);
	}
	else if (preset == 51)
	{
		SetEAXParams(EFX_REVERB_PRESET_ICEPALACE_COURTYARD);
	}
	else if (preset == 52)
	{
		SetEAXParams(EFX_REVERB_PRESET_ICEPALACE_ALCOVE);
	}
	else if (preset == 53)
	{
		SetEAXParams(EFX_REVERB_PRESET_SPACESTATION_SMALLROOM);
	}
	else if (preset == 54)
	{
		SetEAXParams(EFX_REVERB_PRESET_SPACESTATION_SHORTPASSAGE);
	}
	else if (preset == 55)
	{
		SetEAXParams(EFX_REVERB_PRESET_SPACESTATION_MEDIUMROOM);
	}
	else if (preset == 56)
	{
		SetEAXParams(EFX_REVERB_PRESET_SPACESTATION_LARGEROOM);
	}
	else if (preset == 57)
	{
		SetEAXParams(EFX_REVERB_PRESET_SPACESTATION_LONGPASSAGE);
	}
	else if (preset == 58)
	{
		SetEAXParams(EFX_REVERB_PRESET_SPACESTATION_HALL);
	}
	else if (preset == 59)
	{
		SetEAXParams(EFX_REVERB_PRESET_SPACESTATION_CUPBOARD);
	}
	else if (preset == 60)
	{
		SetEAXParams(EFX_REVERB_PRESET_SPACESTATION_ALCOVE);
	}
	else if (preset == 61)
	{
		SetEAXParams(EFX_REVERB_PRESET_WOODEN_SMALLROOM);
	}
	else if (preset == 62)
	{
		SetEAXParams(EFX_REVERB_PRESET_WOODEN_SHORTPASSAGE);
	}
	else if (preset == 63)
	{
		SetEAXParams(EFX_REVERB_PRESET_WOODEN_MEDIUMROOM);
	}
	else if (preset == 64)
	{
		SetEAXParams(EFX_REVERB_PRESET_WOODEN_LARGEROOM);
	}
	else if (preset == 65)
	{
		SetEAXParams(EFX_REVERB_PRESET_WOODEN_LONGPASSAGE);
	}
	else if (preset == 66)
	{
		SetEAXParams(EFX_REVERB_PRESET_WOODEN_HALL);
	}
	else if (preset == 67)
	{
		SetEAXParams(EFX_REVERB_PRESET_WOODEN_CUPBOARD);
	}
	else if (preset == 68)
	{
		SetEAXParams(EFX_REVERB_PRESET_WOODEN_COURTYARD);
	}
	else if (preset == 69)
	{
		SetEAXParams(EFX_REVERB_PRESET_WOODEN_ALCOVE);
	}
	else if (preset == 70)
	{
		SetEAXParams(EFX_REVERB_PRESET_SPORT_EMPTYSTADIUM);
	}
	else if (preset == 71)
	{
		SetEAXParams(EFX_REVERB_PRESET_SPORT_SQUASHCOURT);
	}
	else if (preset == 72)
	{
		SetEAXParams(EFX_REVERB_PRESET_SPORT_SMALLSWIMMINGPOOL);
	}
	else if (preset == 73)
	{
		SetEAXParams(EFX_REVERB_PRESET_SPORT_LARGESWIMMINGPOOL);
	}
	else if (preset == 74)
	{
		SetEAXParams(EFX_REVERB_PRESET_SPORT_GYMNASIUM);
	}
	else if (preset == 75)
	{
		SetEAXParams(EFX_REVERB_PRESET_SPORT_FULLSTADIUM);
	}
	else if (preset == 76)
	{
		SetEAXParams(EFX_REVERB_PRESET_SPORT_STADIUMTANNOY);
	}
	else if (preset == 77)
	{
		SetEAXParams(EFX_REVERB_PRESET_PREFAB_WORKSHOP);
	}
	else if (preset == 78)
	{
		SetEAXParams(EFX_REVERB_PRESET_PREFAB_SCHOOLROOM);
	}
	else if (preset == 79)
	{
		SetEAXParams(EFX_REVERB_PRESET_PREFAB_PRACTISEROOM);
	}
	else if (preset == 80)
	{
		SetEAXParams(EFX_REVERB_PRESET_PREFAB_OUTHOUSE);
	}
	else if (preset == 81)
	{
		SetEAXParams(EFX_REVERB_PRESET_PREFAB_CARAVAN);
	}
	else if (preset == 82)
	{
		SetEAXParams(EFX_REVERB_PRESET_DOME_TOMB);
	}
	else if (preset == 83)
	{
		SetEAXParams(EFX_REVERB_PRESET_PIPE_SMALL);
	}
	else if (preset == 84)
	{
		SetEAXParams(EFX_REVERB_PRESET_DOME_SAINTPAULS);
	}
	else if (preset == 85)
	{
		SetEAXParams(EFX_REVERB_PRESET_PIPE_LONGTHIN);
	}
	else if (preset == 86)
	{
		SetEAXParams(EFX_REVERB_PRESET_PIPE_LARGE);
	}
	else if (preset == 87)
	{
		SetEAXParams(EFX_REVERB_PRESET_PIPE_RESONANT);
	}
	else if (preset == 88)
	{
		SetEAXParams(EFX_REVERB_PRESET_OUTDOORS_BACKYARD);
	}
	else if (preset == 89)
	{
		SetEAXParams(EFX_REVERB_PRESET_OUTDOORS_ROLLINGPLAINS);
	}
	else if (preset == 90)
	{
		SetEAXParams(EFX_REVERB_PRESET_OUTDOORS_DEEPCANYON);
	}
	else if (preset == 91)
	{
		SetEAXParams(EFX_REVERB_PRESET_OUTDOORS_CREEK);
	}
	else if (preset == 92)
	{
		SetEAXParams(EFX_REVERB_PRESET_OUTDOORS_VALLEY);
	}
	else if (preset == 93)
	{
		SetEAXParams(EFX_REVERB_PRESET_MOOD_HEAVEN);
	}
	else if (preset == 94)
	{
		SetEAXParams(EFX_REVERB_PRESET_MOOD_HELL);
	}
	else if (preset == 95)
	{
		SetEAXParams(EFX_REVERB_PRESET_MOOD_MEMORY);
	}
	else if (preset == 96)
	{
		SetEAXParams(EFX_REVERB_PRESET_DRIVING_COMMENTATOR);
	}
	else if (preset == 97)
	{
		SetEAXParams(EFX_REVERB_PRESET_DRIVING_PITGARAGE);
	}
	else if (preset == 98)
	{
		SetEAXParams(EFX_REVERB_PRESET_DRIVING_INCAR_RACER);
	}
	else if (preset == 99)
	{
		SetEAXParams(EFX_REVERB_PRESET_DRIVING_INCAR_SPORTS);
	}
	else if (preset == 100)
	{
		SetEAXParams(EFX_REVERB_PRESET_DRIVING_INCAR_LUXURY);
	}
	else if (preset == 101)
	{
		SetEAXParams(EFX_REVERB_PRESET_DRIVING_FULLGRANDSTAND);
	}
	else if (preset == 102)
	{
		SetEAXParams(EFX_REVERB_PRESET_DRIVING_EMPTYGRANDSTAND);
	}
	else if (preset == 103)
	{
		SetEAXParams(EFX_REVERB_PRESET_DRIVING_TUNNEL);
	}
	else if (preset == 104)
	{
		SetEAXParams(EFX_REVERB_PRESET_CITY_STREETS);
	}
	else if (preset == 105)
	{
		SetEAXParams(EFX_REVERB_PRESET_CITY_SUBWAY);
	}
	else if (preset == 106)
	{
		SetEAXParams(EFX_REVERB_PRESET_CITY_MUSEUM);
	}
	else if (preset == 107)
	{
		SetEAXParams(EFX_REVERB_PRESET_CITY_LIBRARY);
	}
	else if (preset == 108)
	{
		SetEAXParams(EFX_REVERB_PRESET_CITY_UNDERPASS);
	}
	else if (preset == 109)
	{
		SetEAXParams(EFX_REVERB_PRESET_CITY_ABANDONED);
	}
	else if (preset == 110)
	{
		SetEAXParams(EFX_REVERB_PRESET_DUSTYROOM);
	}
	else if (preset == 111)
	{
		SetEAXParams(EFX_REVERB_PRESET_CHAPEL);
	}
	else if (preset == 112)
	{
		SetEAXParams(EFX_REVERB_PRESET_SMALLWATERROOM);
	}
	else
	{
		SetEAXParams(EFX_REVERB_PRESET_GENERIC);
	}
	if (update == true)
	{
		effect.LoadPreset(Density, Diffusion, Gain, GainHF, GainLF, DecayTime, DecayHFRatio, DecayLFRatio, ReflectionsGain, ReflectionsDelay, ReflectionsPanX, ReflectionsPanY, ReflectionsPanZ, LateReverbGain, LateReverbDelay, LateReverbPanX, LateReverbPanY, LateReverbPanZ, EchoTime, EchoDepth, ModulationTime, ModulationDepth, AirAbsorptionGainHF, HFReference, LFReference, RoomRolloffFactor, i_DecayHFLimit);
		effectDouble.LoadPreset(Density, Diffusion, Gain, GainHF, GainLF, DecayTime, DecayHFRatio, DecayLFRatio, ReflectionsGain, ReflectionsDelay, ReflectionsPanX, ReflectionsPanY, ReflectionsPanZ, LateReverbGain, LateReverbDelay, LateReverbPanX, LateReverbPanY, LateReverbPanZ, EchoTime, EchoDepth, ModulationTime, ModulationDepth, AirAbsorptionGainHF, HFReference, LFReference, RoomRolloffFactor, i_DecayHFLimit);
		if (IncorrectMode >= 0.5)	
		{
			effect.Update(int(ReverbRate));
			effectDouble.Update(int(ReverbRate));
		}
		else
		{
			effect.Update(rate);
			effectDouble.Update(rate);
		}
	}
}


char *EAXReverb::GetPresetName(int preset) {
	//get the name of a preset
	if (preset == 0)
	{
		return "GENERIC";
	}
	else if (preset == 1)
	{
		return "PADDEDCELL";
	}
	else if (preset == 2)
	{
		return "ROOM";
	}
	else if (preset == 3)
	{
		return "BATHROOM";
	}
	else if (preset == 4)
	{
		return "LIVINGROOM";
	}
	else if (preset == 5)
	{
		return "STONEROOM";
	}
	else if (preset == 6)
	{
		return "AUDITORIUM";
	}
	else if (preset == 7)
	{
		return "CONCERTHALL";
	}
	else if (preset == 8)
	{
		return "CAVE";
	}
	else if (preset == 9)
	{
		return "ARENA";
	}
	else if (preset == 10)
	{
		return "HANGAR";
	}
	else if (preset == 11)
	{
		return "CARPETEDHALLWAY";
	}
	else if (preset == 12)
	{
		return "HALLWAY";
	}
	else if (preset == 13)
	{
		return "STONECORRIDOR";
	}
	else if (preset == 14)
	{
		return "ALLEY";
	}
	else if (preset == 15)
	{
		return "FOREST";
	}
	else if (preset == 16)
	{
		return "CITY";
	}
	else if (preset == 17)
	{
		return "MOUNTAINS";
	}
	else if (preset == 18)
	{
		return "QUARRY";
	}
	else if (preset == 19)
	{
		return "PLAIN";
	}
	else if (preset == 20)
	{
		return "PARKINGLOT";
	}
	else if (preset == 21)
	{
		return "SEWERPIPE";
	}
	else if (preset == 22)
	{
		return "UNDERWATER";
	}
	else if (preset == 23)
	{
		return "DRUGGED";
	}
	else if (preset == 24)
	{
		return "DIZZY";
	}
	else if (preset == 25)
	{
		return "PSYCHOTIC";
	}
	else if (preset == 26)
	{
		return "CASTLE_SMALLROOM";
	}
	else if (preset == 27)
	{
		return "CASTLE_SHORTPASSAGE";
	}
	else if (preset == 28)
	{
		return "CASTLE_MEDIUMROOM";
	}
	else if (preset == 29)
	{
		return "CASTLE_LARGEROOM";
	}
	else if (preset == 30)
	{
		return "CASTLE_LONGPASSAGE";
	}
	else if (preset == 31)
	{
		return "CASTLE_HALL";
	}
	else if (preset == 32)
	{
		return "CASTLE_CUPBOARD";
	}
	else if (preset == 33)
	{
		return "CASTLE_COURTYARD";
	}
	else if (preset == 34)
	{
		return "CASTLE_ALCOVE";
	}
	else if (preset == 35)
	{
		return "FACTORY_SMALLROOM";
	}
	else if (preset == 36)
	{
		return "FACTORY_SHORTPASSAGE";
	}
	else if (preset == 37)
	{
		return "FACTORY_MEDIUMROOM";
	}
	else if (preset == 38)
	{
		return "FACTORY_LARGEROOM";
	}
	else if (preset == 39)
	{
		return "FACTORY_LONGPASSAGE";
	}
	else if (preset == 40)
	{
		return "FACTORY_HALL";
	}
	else if (preset == 41)
	{
		return "FACTORY_CUPBOARD";
	}
	else if (preset == 42)
	{
		return "FACTORY_COURTYARD";
	}
	else if (preset == 43)
	{
		return "FACTORY_ALCOVE";
	}
	else if (preset == 44)
	{
		return "ICEPALACE_SMALLROOM";
	}
	else if (preset == 45)
	{
		return "ICEPALACE_SHORTPASSAGE";
	}
	else if (preset == 46)
	{
		return "ICEPALACE_MEDIUMROOM";
	}
	else if (preset == 47)
	{
		return "ICEPALACE_LARGEROOM";
	}
	else if (preset == 48)
	{
		return "ICEPALACE_LONGPASSAGE";
	}
	else if (preset == 49)
	{
		return "ICEPALACE_HALL";
	}
	else if (preset == 50)
	{
		return "ICEPALACE_CUPBOARD";
	}
	else if (preset == 51)
	{
		return "ICEPALACE_COURTYARD";
	}
	else if (preset == 52)
	{
		return "ICEPALACE_ALCOVE";
	}
	else if (preset == 53)
	{
		return "SPACESTATION_SMALLROOM";
	}
	else if (preset == 54)
	{
		return "SPACESTATION_SHORTPASSAGE";
	}
	else if (preset == 55)
	{
		return "SPACESTATION_MEDIUMROOM";
	}
	else if (preset == 56)
	{
		return "SPACESTATION_LARGEROOM";
	}
	else if (preset == 57)
	{
		return "SPACESTATION_LONGPASSAGE";
	}
	else if (preset == 58)
	{
		return "SPACESTATION_HALL";
	}
	else if (preset == 59)
	{
		return "SPACESTATION_CUPBOARD";
	}
	else if (preset == 60)
	{
		return "SPACESTATION_ALCOVE";
	}
	else if (preset == 61)
	{
		return "WOODEN_SMALLROOM";
	}
	else if (preset == 62)
	{
		return "WOODEN_SHORTPASSAGE";
	}
	else if (preset == 63)
	{
		return "WOODEN_MEDIUMROOM";
	}
	else if (preset == 64)
	{
		return "WOODEN_LARGEROOM";
	}
	else if (preset == 65)
	{
		return "WOODEN_LONGPASSAGE";
	}
	else if (preset == 66)
	{
		return "WOODEN_HALL";
	}
	else if (preset == 67)
	{
		return "WOODEN_CUPBOARD";
	}
	else if (preset == 68)
	{
		return "WOODEN_COURTYARD";
	}
	else if (preset == 69)
	{
		return "WOODEN_ALCOVE";
	}
	else if (preset == 70)
	{
		return "SPORT_EMPTYSTADIUM";
	}
	else if (preset == 71)
	{
		return "SPORT_SQUASHCOURT";
	}
	else if (preset == 72)
	{
		return "SPORT_SMALLSWIMMINGPOOL";
	}
	else if (preset == 73)
	{
		return "SPORT_LARGESWIMMINGPOOL";
	}
	else if (preset == 74)
	{
		return "SPORT_GYMNASIUM";
	}
	else if (preset == 75)
	{
		return "SPORT_FULLSTADIUM";
	}
	else if (preset == 76)
	{
		return "SPORT_STADIUMTANNOY";
	}
	else if (preset == 77)
	{
		return "PREFAB_WORKSHOP";
	}
	else if (preset == 78)
	{
		return "PREFAB_SCHOOLROOM";
	}
	else if (preset == 79)
	{
		return "PREFAB_PRACTISEROOM";
	}
	else if (preset == 80)
	{
		return "PREFAB_OUTHOUSE";
	}
	else if (preset == 81)
	{
		return "PREFAB_CARAVAN";
	}
	else if (preset == 82)
	{
		return "DOME_TOMB";
	}
	else if (preset == 83)
	{
		return "PIPE_SMALL";
	}
	else if (preset == 84)
	{
		return "DOME_SAINTPAULS";
	}
	else if (preset == 85)
	{
		return "PIPE_LONGTHIN";
	}
	else if (preset == 86)
	{
		return "PIPE_LARGE";
	}
	else if (preset == 87)
	{
		return "PIPE_RESONANT";
	}
	else if (preset == 88)
	{
		return "OUTDOORS_BACKYARD";
	}
	else if (preset == 89)
	{
		return "OUTDOORS_ROLLINGPLAINS";
	}
	else if (preset == 90)
	{
		return "OUTDOORS_DEEPCANYON";
	}
	else if (preset == 91)
	{
		return "OUTDOORS_CREEK";
	}
	else if (preset == 92)
	{
		return "OUTDOORS_VALLEY";
	}
	else if (preset == 93)
	{
		return "MOOD_HEAVEN";
	}
	else if (preset == 94)
	{
		return "MOOD_HELL";
	}
	else if (preset == 95)
	{
		return "MOOD_MEMORY";
	}
	else if (preset == 96)
	{
		return "DRIVING_COMMENTATOR";
	}
	else if (preset == 97)
	{
		return "DRIVING_PITGARAGE";
	}
	else if (preset == 98)
	{
		return "DRIVING_INCAR_RACER";
	}
	else if (preset == 99)
	{
		return "DRIVING_INCAR_SPORTS";
	}
	else if (preset == 100)
	{
		return "DRIVING_INCAR_LUXURY";
	}
	else if (preset == 101)
	{
		return "DRIVING_FULLGRANDSTAND";
	}
	else if (preset == 102)
	{
		return "DRIVING_EMPTYGRANDSTAND";
	}
	else if (preset == 103)
	{
		return "DRIVING_TUNNEL";
	}
	else if (preset == 104)
	{
		return "CITY_STREETS";
	}
	else if (preset == 105)
	{
		return "CITY_SUBWAY";
	}
	else if (preset == 106)
	{
		return "CITY_MUSEUM";
	}
	else if (preset == 107)
	{
		return "CITY_LIBRARY";
	}
	else if (preset == 108)
	{
		return "CITY_UNDERPASS";
	}
	else if (preset == 109)
	{
		return "CITY_ABANDONED";
	}
	else if (preset == 110)
	{
		return "DUSTYROOM";
	}
	else if (preset == 111)
	{
		return "CHAPEL";
	}
	else if (preset == 112)
	{
		return "SMALLWATERROOM";
	}
	return "GENERIC";
}


void EAXReverb::SetDensity (float val)
{
	if (val > EAXREVERB_MAX_DENSITY)
	{
		val = EAXREVERB_MAX_DENSITY;
	}
	else if (val < EAXREVERB_MIN_DENSITY)
	{
		val = EAXREVERB_MIN_DENSITY;
	}
	Density = val;
	programs[curProgram].Density = val;
}


void EAXReverb::SetDiffusion (float val)
{
	if (val > EAXREVERB_MAX_DIFFUSION)
	{
		val = EAXREVERB_MAX_DIFFUSION;
	}
	else if (val < EAXREVERB_MIN_DIFFUSION)
	{
		val = EAXREVERB_MIN_DIFFUSION;
	}
	Diffusion = val;
	programs[curProgram].Diffusion = val;
}


void EAXReverb::SetGain (float val)
{
	if (val > EAXREVERB_MAX_GAIN)
	{
		val = EAXREVERB_MAX_GAIN;
	}
	else if (val < EAXREVERB_MIN_GAIN)
	{
		val = EAXREVERB_MIN_GAIN;
	}
	Gain = val;
	programs[curProgram].Gain = val;
}


void EAXReverb::SetGainHF (float val)
{
	if (val > EAXREVERB_MAX_GAINHF)
	{
		val = EAXREVERB_MAX_GAINHF;
	}
	else if (val < EAXREVERB_MIN_GAINHF)
	{
		val = EAXREVERB_MIN_GAINHF;
	}
	GainHF = val;
	programs[curProgram].GainHF = val;
}


void EAXReverb::SetGainLF (float val)
{
	if (val > EAXREVERB_MAX_GAINLF)
	{
		val = EAXREVERB_MAX_GAINLF;
	}
	else if (val < EAXREVERB_MIN_GAINLF)
	{
		val = EAXREVERB_MIN_GAINLF;
	}
	GainLF = val;
	programs[curProgram].GainLF = val;
}


void EAXReverb::SetDecayTime (float val)
{
	if (val > EAXREVERB_MAX_DECAY_TIME)
	{
		val = EAXREVERB_MAX_DECAY_TIME;
	}
	else if (val < EAXREVERB_MIN_DECAY_TIME)
	{
		val = EAXREVERB_MIN_DECAY_TIME;
	}
	DecayTime = val;
	programs[curProgram].DecayTime = val;
}


void EAXReverb::SetDecayHFRatio (float val)
{
	if (val > EAXREVERB_MAX_DECAY_HFRATIO)
	{
		val = EAXREVERB_MAX_DECAY_HFRATIO;
	}
	else if (val < EAXREVERB_MIN_DECAY_HFRATIO)
	{
		val = EAXREVERB_MIN_DECAY_HFRATIO;
	}
	DecayHFRatio = val;
	programs[curProgram].DecayHFRatio = val;
}


void EAXReverb::SetDecayLFRatio (float val)
{
	if (val > EAXREVERB_MAX_DECAY_LFRATIO)
	{
		val = EAXREVERB_MAX_DECAY_LFRATIO;
	}
	else if (val < EAXREVERB_MIN_DECAY_LFRATIO)
	{
		val = EAXREVERB_MIN_DECAY_LFRATIO;
	}
	DecayLFRatio = val;
	programs[curProgram].DecayLFRatio = val;
}


void EAXReverb::SetReflectionsGain (float val)
{
	if (val > EAXREVERB_MAX_REFLECTIONS_GAIN)
	{
		val = EAXREVERB_MAX_REFLECTIONS_GAIN;
	}
	else if (val < EAXREVERB_MIN_REFLECTIONS_GAIN)
	{
		val = EAXREVERB_MIN_REFLECTIONS_GAIN;
	}
	ReflectionsGain = val;
	programs[curProgram].ReflectionsGain = val;
}


void EAXReverb::SetReflectionsDelay (float val)
{
	if (val > EAXREVERB_MAX_REFLECTIONS_DELAY)
	{
		val = EAXREVERB_MAX_REFLECTIONS_DELAY;
	}
	else if (val < EAXREVERB_MIN_REFLECTIONS_DELAY)
	{
		val = EAXREVERB_MIN_REFLECTIONS_DELAY;
	}
	ReflectionsDelay = val;
	programs[curProgram].ReflectionsDelay = val;
}


void EAXReverb::SetReflectionsPanX (float val)
{
	ReflectionsPanX = val;
	programs[curProgram].ReflectionsPanX = val;
}


void EAXReverb::SetReflectionsPanY (float val)
{
	ReflectionsPanY = val;
	programs[curProgram].ReflectionsPanY = val;
}


void EAXReverb::SetReflectionsPanZ (float val)
{
	ReflectionsPanZ = val;
	programs[curProgram].ReflectionsPanZ = val;
}


void EAXReverb::SetLateReverbGain (float val)
{
	if (val > EAXREVERB_MAX_LATE_REVERB_GAIN)
	{
		val = EAXREVERB_MAX_LATE_REVERB_GAIN;
	}
	else if (val < EAXREVERB_MIN_LATE_REVERB_GAIN)
	{
		val = EAXREVERB_MIN_LATE_REVERB_GAIN;
	}
	LateReverbGain = val;
	programs[curProgram].LateReverbGain = val;
}


void EAXReverb::SetLateReverbDelay (float val)
{
	if (val > EAXREVERB_MAX_LATE_REVERB_DELAY)
	{
		val = EAXREVERB_MAX_LATE_REVERB_DELAY;
	}
	else if (val < EAXREVERB_MIN_LATE_REVERB_DELAY)
	{
		val = EAXREVERB_MIN_LATE_REVERB_DELAY;
	}
	LateReverbDelay = val;
	programs[curProgram].LateReverbDelay = val;
}


void EAXReverb::SetLateReverbPanX (float val)
{
	LateReverbPanX = val;
	programs[curProgram].LateReverbPanX = val;
}


void EAXReverb::SetLateReverbPanY (float val)
{
	LateReverbPanY = val;
	programs[curProgram].LateReverbPanY = val;
}


void EAXReverb::SetLateReverbPanZ (float val)
{
	LateReverbPanZ = val;
	programs[curProgram].LateReverbPanZ = val;
}


void EAXReverb::SetEchoTime (float val)
{
	if (val > EAXREVERB_MAX_ECHO_TIME)
	{
		val = EAXREVERB_MAX_ECHO_TIME;
	}
	else if (val < EAXREVERB_MIN_ECHO_TIME)
	{
		val = EAXREVERB_MIN_ECHO_TIME;
	}
	EchoTime = val;
	programs[curProgram].EchoTime = val;
}


void EAXReverb::SetEchoDepth (float val)
{
	if (val > EAXREVERB_MAX_ECHO_DEPTH)
	{
		val = EAXREVERB_MAX_ECHO_DEPTH;
	}
	else if (val < EAXREVERB_MIN_ECHO_DEPTH)
	{
		val = EAXREVERB_MIN_ECHO_DEPTH;
	}
	EchoDepth = val;
	programs[curProgram].EchoDepth = val;
}


void EAXReverb::SetModulationTime (float val)
{
	if (val > EAXREVERB_MAX_MODULATION_TIME)
	{
		val = EAXREVERB_MAX_MODULATION_TIME;
	}
	else if (val < EAXREVERB_MIN_MODULATION_TIME)
	{
		val = EAXREVERB_MIN_MODULATION_TIME;
	}
	ModulationTime = val;
	programs[curProgram].ModulationTime = val;
}


void EAXReverb::SetModulationDepth (float val)
{
	if (val > EAXREVERB_MAX_MODULATION_DEPTH)
	{
		val = EAXREVERB_MAX_MODULATION_DEPTH;
	}
	else if (val < EAXREVERB_MIN_MODULATION_DEPTH)
	{
		val = EAXREVERB_MIN_MODULATION_DEPTH;
	}
	ModulationDepth = val;
	programs[curProgram].ModulationDepth = val;
}


void EAXReverb::SetAirAbsorptionGainHF (float val)
{
	if (val > EAXREVERB_MAX_AIR_ABSORPTION_GAINHF)
	{
		val = EAXREVERB_MAX_AIR_ABSORPTION_GAINHF;
	}
	else if (val < EAXREVERB_MIN_AIR_ABSORPTION_GAINHF)
	{
		val = EAXREVERB_MIN_AIR_ABSORPTION_GAINHF;
	}
	AirAbsorptionGainHF = val;
	programs[curProgram].AirAbsorptionGainHF = val;
}


void EAXReverb::SetHFReference (float val)
{
	if (val > EAXREVERB_MAX_HFREFERENCE)
	{
		val = EAXREVERB_MAX_HFREFERENCE;
	}
	else if (val < EAXREVERB_MIN_HFREFERENCE)
	{
		val = EAXREVERB_MIN_HFREFERENCE;
	}
	HFReference = val;
	programs[curProgram].HFReference = val;
}


void EAXReverb::SetLFReference (float val)
{
	if (val > EAXREVERB_MAX_LFREFERENCE)
	{
		val = EAXREVERB_MAX_LFREFERENCE;
	}
	else if (val < EAXREVERB_MIN_LFREFERENCE)
	{
		val = EAXREVERB_MIN_LFREFERENCE;
	}
	LFReference = val;
	programs[curProgram].LFReference = val;
}


void EAXReverb::SetRoomRolloffFactor (float val)
{
	if (val > EAXREVERB_MAX_ROOM_ROLLOFF_FACTOR)
	{
		val = EAXREVERB_MAX_ROOM_ROLLOFF_FACTOR;
	}
	else if (val < EAXREVERB_MIN_ROOM_ROLLOFF_FACTOR)
	{
		val = EAXREVERB_MIN_ROOM_ROLLOFF_FACTOR;
	}
	RoomRolloffFactor = val;
	programs[curProgram].RoomRolloffFactor = val;
}


void EAXReverb::SetDecayHFLimit (float val)
{
	DecayHFLimit = val;
	programs[curProgram].DecayHFLimit = val;
	if (DecayHFLimit >= 0.5)
	{
		i_DecayHFLimit = EAXREVERB_MAX_DECAY_HFLIMIT;
	}
	else
	{
		i_DecayHFLimit = EAXREVERB_MIN_DECAY_HFLIMIT;
	}
}


void EAXReverb::SetEAXParams(float Density, float Diffusion, float Gain, float GainHF, float GainLF, float DecayTime, float DecayHFRatio, float DecayLFRatio, float ReflectionsGain, float ReflectionsDelay, float ReflectionsPanX, float ReflectionsPanY, float ReflectionsPanZ, float LateReverbGain, float LateReverbDelay, float LateReverbPanX, float LateReverbPanY, float LateReverbPanZ, float EchoTime, float EchoDepth, float ModulationTime, float ModulationDepth, float AirAbsorptionGainHF, float HFReference, float LFReference, float RoomRolloffFactor, float DecayHFLimit)
{
	SetDensity(Density);
	SetDiffusion(Diffusion);
	SetGain(Gain);
	SetGainHF(GainHF);
	SetGainLF(GainLF);
	SetDecayTime(DecayTime);
	SetDecayHFRatio(DecayHFRatio);
	SetDecayLFRatio(DecayLFRatio);
	SetReflectionsGain(ReflectionsGain);
	SetReflectionsDelay(ReflectionsDelay);
	SetReflectionsPanX(ReflectionsPanX);
	SetReflectionsPanY(ReflectionsPanY);
	SetReflectionsPanZ(ReflectionsPanZ);
	SetLateReverbGain(LateReverbGain);
	SetLateReverbDelay(LateReverbDelay);
	SetLateReverbPanX(LateReverbPanX);
	SetLateReverbPanY(LateReverbPanY);
	SetLateReverbPanZ(LateReverbPanZ);
	SetEchoTime(EchoTime);
	SetEchoDepth(EchoDepth);
	SetModulationTime(ModulationTime);
	SetModulationDepth(ModulationDepth);
	SetAirAbsorptionGainHF(AirAbsorptionGainHF);
	SetHFReference(HFReference);
	SetLFReference(LFReference);
	SetRoomRolloffFactor(RoomRolloffFactor);
	SetDecayHFLimit(DecayHFLimit);
}


void EAXReverb::setProgramName (char *name)
{
	strcpy (programs[curProgram].name, name);
}


void EAXReverb::getProgramName (char *name)
{
	strcpy (name, programs[curProgram].name);
}


bool EAXReverb::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
	if (index < kNumPrograms)
	{
		strcpy (text, programs[index].name);
		return true;
	}
	return false;
}


void EAXReverb::resume ()
{
	rate = int(AudioEffectX::updateSampleRate());
	effect.Create(rate);
	effectDouble.Create(rate);
	effect.LoadPreset(Density, Diffusion, Gain, GainHF, GainLF, DecayTime, DecayHFRatio, DecayLFRatio, ReflectionsGain, ReflectionsDelay, ReflectionsPanX, ReflectionsPanY, ReflectionsPanZ, LateReverbGain, LateReverbDelay, LateReverbPanX, LateReverbPanY, LateReverbPanZ, EchoTime, EchoDepth, ModulationTime, ModulationDepth, AirAbsorptionGainHF, HFReference, LFReference, RoomRolloffFactor, i_DecayHFLimit);
	effectDouble.LoadPreset(Density, Diffusion, Gain, GainHF, GainLF, DecayTime, DecayHFRatio, DecayLFRatio, ReflectionsGain, ReflectionsDelay, ReflectionsPanX, ReflectionsPanY, ReflectionsPanZ, LateReverbGain, LateReverbDelay, LateReverbPanX, LateReverbPanY, LateReverbPanZ, EchoTime, EchoDepth, ModulationTime, ModulationDepth, AirAbsorptionGainHF, HFReference, LFReference, RoomRolloffFactor, i_DecayHFLimit);
	if (IncorrectMode >= 0.5)	
	{
		effect.Update(int(ReverbRate));
		effectDouble.Update(int(ReverbRate));
	}
	else
	{
		effect.Update(rate);
		effectDouble.Update(rate);
	}
	i_ReverbPreset = int(ReverbPreset);
	if (FLTFreq > rate/2-2)
	{
		FLTFreq = float(rate/2-2);
	}
	if (FLTType >= 0.0 && FLTType < 0.125)	
	{
		sf_lowpass(&bq_state, rate, FLTFreq/2, FLTRes);
		sfd_lowpass(&bq_state_double, rate, FLTFreq/2, FLTRes);
	}
	else if (FLTType >= 0.125 && FLTType < 0.25)	
	{
		sf_highpass(&bq_state, rate, FLTFreq/2, FLTRes);
		sfd_highpass(&bq_state_double, rate, FLTFreq/2, FLTRes);
	}
	else if (FLTType >= 0.25 && FLTType < 0.375)	
	{
		sf_bandpass(&bq_state, rate, FLTFreq/2, FLTRes);
		sfd_bandpass(&bq_state_double, rate, FLTFreq/2, FLTRes);
	}
	else if (FLTType >= 0.375 && FLTType < 0.5)	
	{
		sf_allpass(&bq_state, rate, FLTFreq/2, FLTRes);
		sfd_allpass(&bq_state_double, rate, FLTFreq/2, FLTRes);
	}
	else if (FLTType >= 0.5 && FLTType < 0.625)	
	{
		sf_notch(&bq_state, rate, FLTFreq/2, FLTRes);
		sfd_notch(&bq_state_double, rate, FLTFreq/2, FLTRes);
	}
	else if (FLTType >= 0.625 && FLTType < 0.75)	
	{
		sf_peaking(&bq_state, rate, FLTFreq/2, FLTRes, FLTGain);
		sfd_peaking(&bq_state_double, rate, FLTFreq/2, FLTRes, FLTGain);
	}
	else if (FLTType >= 0.75 && FLTType < 0.875)	
	{
		sf_lowshelf(&bq_state, rate, FLTFreq/2, FLTRes, FLTGain);
		sfd_lowshelf(&bq_state_double, rate, FLTFreq/2, FLTRes, FLTGain);
	}
	else if (FLTType >= 0.875 && FLTType <= 1.0)	
	{
		sf_highshelf(&bq_state, rate, FLTFreq/2, FLTRes, FLTGain);
		sfd_highshelf(&bq_state_double, rate, FLTFreq/2, FLTRes, FLTGain);
	}
	else
	{
		sf_highpass(&bq_state, rate, 0, 0);
		sfd_highpass(&bq_state_double, rate, 0, 0);
	}
	linearresampler1 = LinearResamplerCreate();
	linearresampler2 = LinearResamplerCreate();
	resampler1 = resampler_create();
	resampler2 = resampler_create();
	zohresampler1 = ZOHResamplerCreate();
	zohresampler2 = ZOHResamplerCreate();
	if (RSMRate >= 1.0)
	{
		rsm = int(RSMRate);
	}
	else
	{
		rsm = 1;
	}
	if (RSMMode >= 0.5)
	{
		rate_new = rate*rsm;
	}
	else
	{
		rate_new = rate/rsm;
	}
	LinearResamplerSetup(linearresampler1, rate, rate_new);
	LinearResamplerSetup(linearresampler2, rate_new, rate);
	resampler_set_rate(resampler1, (double)rate / (double)rate_new);
	resampler_set_rate(resampler2, (double)rate_new / (double)rate);
	ZOHResamplerSetup(zohresampler1, rate, rate_new);
	ZOHResamplerSetup(zohresampler2, rate_new, rate);
	AudioEffectX::resume();
}


VstInt32 EAXReverb::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	for (int i = 0; i < kNumParams; i++)
	{
		setParameter(i, chunkData[i]);
	}
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	return 0;
}


VstInt32 EAXReverb::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParams, sizeof(float));
	for (int i = 0; i < kNumParams; i++)
	{
		chunkData[i] = getParameter(i);
	}
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	started with. */
	
	*data = chunkData;
	return kNumParams * sizeof(float);
}


void EAXReverb::setParameter (VstInt32 index, float value)
{
	switch (index)
	{
	case kDisable :    SetDisableEffect (value);					break;
	case kMute :    SetMuteEffect (value);					break;
	case kInvertmode :    SetInvertMode (value);					break;
	case kMonomode :    SetMonoMode (value);					break;
	case kInvertorig :    SetInvertOriginal (value);					break;
	case kInvertrev :    SetInvertReverb (value);					break;
	case kInvert :    SetInvert (value);					break;
	case kSwaporig :    SetSwapOriginal (value);					break;
	case kSwaprev :    SetSwapReverb (value);					break;
	case kSwap :    SetSwap (value);					break;
	case kBalancelorig :    SetBalanceLOriginal (value);					break;
	case kBalancerorig :    SetBalanceROriginal (value);					break;
	case kBalancelrev :    SetBalanceLReverb (value);					break;
	case kBalancerrev :    SetBalanceRReverb (value);					break;
	case kBalancel :    SetBalanceL (value);					break;
	case kBalancer :    SetBalanceR (value);					break;
	case kStereoorig :    SetStereoWidthOriginal (value);					break;
	case kStereorev :    SetStereoWidthReverb (value);					break;
	case kStereo :    SetStereoWidth (value);					break;
	case kMonoorig :    SetMonoOriginal (value);					break;
	case kMonorev :    SetMonoReverb (value);					break;
	case kMono :    SetMono (value);					break;
	case kOnlyorig :    SetOnlyOriginal (value);					break;
	case kOnlyrev :    SetOnlyReverb (value);					break;
	case kMix :    SetMixMode (value);					break;
	case kDgain :    SetDryGain (value);					break;
	case kWgain :    SetWetGain (value);					break;
	case kMgain :    SetMasterGain (value);					break;
	case kResample :    SetResample (value);					break;
	case kRsmq :    SetRSMQ (value);					break;
	case kRsmmode :    SetRSMMode (value);					break;
	case kRsmrate :    SetRSMRate (value*64);					break;
	case kBitcrush :    SetBitCrush (value);					break;
	case kBitdepth :    SetBitDepth (value*16);					break;
	case kDither :    SetDither (value);					break;
	case kAutodither :    SetAutoD (value);					break;
	case kOnlyqerror :    SetOnlyQError (value);					break;
	case kLimit :    SetLimit (value);					break;
	case kBqfilter :    SetBQFilter (value);					break;
	case kFlttype :    SetFLTType (value);					break;
	case kFltfreq :    SetFLTFreq (value*20000);					break;
	case kFltres :    SetFLTRes (value*10);					break;
	case kFltgain :    SetFLTGain (value*10);					break;
	case kIncorrect :    SetIncorrectMode (value);					break;
	case kRate :    SetReverbRate (value*1000000);					break;
	case kPreset :    SetReverbPreset (int(value*1000.f+0.0005f), true);					break;
	case kDensity :    SetDensity (value*EAXREVERB_MAX_DENSITY);					break;
	case kDiffusion :    SetDiffusion (value*EAXREVERB_MAX_DIFFUSION);					break;
	case kGain :    SetGain (value*EAXREVERB_MAX_GAIN);					break;
	case kGainHF :    SetGainHF (value*EAXREVERB_MAX_GAINHF);					break;
	case kGainLF :    SetGainLF (value*EAXREVERB_MAX_GAINLF);					break;
	case kDecayTime :    SetDecayTime (value*EAXREVERB_MAX_DECAY_TIME);					break;
	case kDecayHFRatio :    SetDecayHFRatio (value*EAXREVERB_MAX_DECAY_HFRATIO);					break;
	case kDecayLFRatio :    SetDecayLFRatio (value*EAXREVERB_MAX_DECAY_LFRATIO);					break;
	case kReflectionsGain :    SetReflectionsGain (value*EAXREVERB_MAX_REFLECTIONS_GAIN);					break;
	case kReflectionsDelay :    SetReflectionsDelay (value*EAXREVERB_MAX_REFLECTIONS_DELAY);					break;
	case kReflectionsPanX :    SetReflectionsPanX (value);					break;
	case kReflectionsPanY :    SetReflectionsPanY (value);					break;
	case kReflectionsPanZ :    SetReflectionsPanZ (value);					break;
	case kLateReverbGain :    SetLateReverbGain (value*EAXREVERB_MAX_LATE_REVERB_GAIN);					break;
	case kLateReverbDelay :    SetLateReverbDelay (value*EAXREVERB_MAX_LATE_REVERB_DELAY);					break;
	case kLateReverbPanX :    SetLateReverbPanX (value);					break;
	case kLateReverbPanY :    SetLateReverbPanY (value);					break;
	case kLateReverbPanZ :    SetLateReverbPanZ (value);					break;
	case kEchoTime :    SetEchoTime (value*EAXREVERB_MAX_ECHO_TIME);					break;
	case kEchoDepth :    SetEchoDepth (value*EAXREVERB_MAX_ECHO_DEPTH);					break;
	case kModulationTime :    SetModulationTime (value*EAXREVERB_MAX_MODULATION_TIME);					break;
	case kModulationDepth :    SetModulationDepth (value*EAXREVERB_MAX_MODULATION_DEPTH);					break;
	case kAirAbsorptionGainHF :    SetAirAbsorptionGainHF (value*EAXREVERB_MAX_AIR_ABSORPTION_GAINHF);					break;
	case kHFReference :    SetHFReference (value*EAXREVERB_MAX_HFREFERENCE);					break;
	case kLFReference :    SetLFReference (value*EAXREVERB_MAX_LFREFERENCE);					break;
	case kRoomRolloffFactor :    SetRoomRolloffFactor (value*EAXREVERB_MAX_ROOM_ROLLOFF_FACTOR);					break;
	case kDecayHFLimit :    SetDecayHFLimit (value);					break;
	}
	if (index >= kFlttype && index <= kFltgain)
	{
		if (FLTType >= 0.0 && FLTType < 0.125)	
		{
			sf_lowpass(&bq_state, rate, FLTFreq/2, FLTRes);
			sfd_lowpass(&bq_state_double, rate, FLTFreq/2, FLTRes);
		}
		else if (FLTType >= 0.125 && FLTType < 0.25)	
		{
			sf_highpass(&bq_state, rate, FLTFreq/2, FLTRes);
			sfd_highpass(&bq_state_double, rate, FLTFreq/2, FLTRes);
		}
		else if (FLTType >= 0.25 && FLTType < 0.375)	
		{
			sf_bandpass(&bq_state, rate, FLTFreq/2, FLTRes);
			sfd_bandpass(&bq_state_double, rate, FLTFreq/2, FLTRes);
		}
		else if (FLTType >= 0.375 && FLTType < 0.5)	
		{
			sf_allpass(&bq_state, rate, FLTFreq/2, FLTRes);
			sfd_allpass(&bq_state_double, rate, FLTFreq/2, FLTRes);
		}
		else if (FLTType >= 0.5 && FLTType < 0.625)	
		{
			sf_notch(&bq_state, rate, FLTFreq/2, FLTRes);
			sfd_notch(&bq_state_double, rate, FLTFreq/2, FLTRes);
		}
		else if (FLTType >= 0.625 && FLTType < 0.75)	
		{
			sf_peaking(&bq_state, rate, FLTFreq/2, FLTRes, FLTGain);
			sfd_peaking(&bq_state_double, rate, FLTFreq/2, FLTRes, FLTGain);
		}
		else if (FLTType >= 0.75 && FLTType < 0.875)	
		{
			sf_lowshelf(&bq_state, rate, FLTFreq/2, FLTRes, FLTGain);
			sfd_lowshelf(&bq_state_double, rate, FLTFreq/2, FLTRes, FLTGain);
		}
		else if (FLTType >= 0.875 && FLTType <= 1.0)	
		{
			sf_highshelf(&bq_state, rate, FLTFreq/2, FLTRes, FLTGain);
			sfd_highshelf(&bq_state_double, rate, FLTFreq/2, FLTRes, FLTGain);
		}
		else
		{
			sf_highpass(&bq_state, rate, 0, 0);
			sfd_highpass(&bq_state_double, rate, 0, 0);
		}
	}
	if (index > kPreset && index < kNumParams)
	{
		effect.LoadPreset(Density, Diffusion, Gain, GainHF, GainLF, DecayTime, DecayHFRatio, DecayLFRatio, ReflectionsGain, ReflectionsDelay, ReflectionsPanX, ReflectionsPanY, ReflectionsPanZ, LateReverbGain, LateReverbDelay, LateReverbPanX, LateReverbPanY, LateReverbPanZ, EchoTime, EchoDepth, ModulationTime, ModulationDepth, AirAbsorptionGainHF, HFReference, LFReference, RoomRolloffFactor, i_DecayHFLimit);
		effectDouble.LoadPreset(Density, Diffusion, Gain, GainHF, GainLF, DecayTime, DecayHFRatio, DecayLFRatio, ReflectionsGain, ReflectionsDelay, ReflectionsPanX, ReflectionsPanY, ReflectionsPanZ, LateReverbGain, LateReverbDelay, LateReverbPanX, LateReverbPanY, LateReverbPanZ, EchoTime, EchoDepth, ModulationTime, ModulationDepth, AirAbsorptionGainHF, HFReference, LFReference, RoomRolloffFactor, i_DecayHFLimit);
		if (IncorrectMode >= 0.5)	
		{
			effect.Update(int(ReverbRate));
			effectDouble.Update(int(ReverbRate));
		}
		else
		{
			effect.Update(rate);
			effectDouble.Update(rate);
		}
	}
}


void EAXReverb::setParameterAutomated (VstInt32 index, float value)
{
	setParameter(index, value);
}


float EAXReverb::getParameter (VstInt32 index)
{
	float v = 0;

	switch (index)
	{
	case kDisable :    v = DisableEffect;	break;
	case kMute :    v = MuteEffect;	break;
	case kInvertmode :    v = InvertMode;	break;
	case kMonomode :    v = MonoMode;	break;
	case kInvertorig :    v = InvertOriginal;	break;
	case kInvertrev :    v = InvertReverb;	break;
	case kInvert :    v = Invert;	break;
	case kSwaporig :    v = SwapOriginal;	break;
	case kSwaprev :    v = SwapReverb;	break;
	case kSwap :    v = Swap;	break;
	case kBalancelorig :    v = BalanceLOriginal;	break;
	case kBalancerorig :    v = BalanceROriginal;	break;
	case kBalancelrev :    v = BalanceLReverb;	break;
	case kBalancerrev :    v = BalanceRReverb;	break;
	case kBalancel :    v = BalanceL;	break;
	case kBalancer :    v = BalanceR;	break;
	case kStereoorig :    v = StereoWidthOriginal;	break;
	case kStereorev :    v = StereoWidthReverb;	break;
	case kStereo :    v = StereoWidth;	break;
	case kMonoorig :    v = MonoOriginal;	break;
	case kMonorev :    v = MonoReverb;	break;
	case kMono :    v = Mono;	break;
	case kOnlyorig :    v = OnlyOriginal;	break;
	case kOnlyrev :    v = OnlyReverb;	break;
	case kMix :    v = MixMode;	break;
	case kDgain :    v = DryGain;	break;
	case kWgain :    v = WetGain;	break;
	case kMgain :    v = MasterGain;	break;
	case kResample :    v = Resample;	break;
	case kRsmq :    v = RSMQ;	break;
	case kRsmmode :    v = RSMMode;	break;
	case kRsmrate :    v = RSMRate/64;	break;
	case kBitcrush :    v = BitCr;	break;
	case kBitdepth :    v = BitDepth/16;	break;
	case kDither :    v = Dither;	break;
	case kAutodither :    v = AutoDither;	break;
	case kOnlyqerror :    v = OnlyQError;	break;
	case kLimit :    v = Limit;	break;
	case kBqfilter :    v = BQFilter;	break;
	case kFlttype :    v = FLTType;	break;
	case kFltfreq :    v = FLTFreq/20000;	break;
	case kFltres :    v = FLTRes/10;	break;
	case kFltgain :    v = FLTGain/10;	break;
	case kIncorrect :    v = IncorrectMode;	break;
	case kRate :    v = ReverbRate/1000000;	break;
	case kPreset :    v = ReverbPreset/1000.f+0.0005f;	break;
	case kDensity :    v = Density/EAXREVERB_MAX_DENSITY;	break;
	case kDiffusion :    v = Diffusion/EAXREVERB_MAX_DIFFUSION;	break;
	case kGain :    v = Gain/EAXREVERB_MAX_GAIN;	break;
	case kGainHF :    v = GainHF/EAXREVERB_MAX_GAINHF;	break;
	case kGainLF :    v = GainLF/EAXREVERB_MAX_GAINLF;	break;
	case kDecayTime :    v = DecayTime/EAXREVERB_MAX_DECAY_TIME;	break;
	case kDecayHFRatio :    v = DecayHFRatio/EAXREVERB_MAX_DECAY_HFRATIO;	break;
	case kDecayLFRatio :    v = DecayLFRatio/EAXREVERB_MAX_DECAY_LFRATIO;	break;
	case kReflectionsGain :    v = ReflectionsGain/EAXREVERB_MAX_REFLECTIONS_GAIN;	break;
	case kReflectionsDelay :    v = ReflectionsDelay/EAXREVERB_MAX_REFLECTIONS_DELAY;	break;
	case kReflectionsPanX :    v = ReflectionsPanX;	break;
	case kReflectionsPanY :    v = ReflectionsPanY;	break;
	case kReflectionsPanZ :    v = ReflectionsPanZ;	break;
	case kLateReverbGain :    v = LateReverbGain/EAXREVERB_MAX_LATE_REVERB_GAIN;	break;
	case kLateReverbDelay :    v = LateReverbDelay/EAXREVERB_MAX_LATE_REVERB_DELAY;	break;
	case kLateReverbPanX :    v = LateReverbPanX;	break;
	case kLateReverbPanY :    v = LateReverbPanY;	break;
	case kLateReverbPanZ :    v = LateReverbPanZ;	break;
	case kEchoTime :    v = EchoTime/EAXREVERB_MAX_ECHO_TIME;	break;
	case kEchoDepth :    v = EchoDepth/EAXREVERB_MAX_ECHO_DEPTH;	break;
	case kModulationTime :    v = ModulationTime/EAXREVERB_MAX_MODULATION_TIME;	break;
	case kModulationDepth :    v = ModulationDepth/EAXREVERB_MAX_MODULATION_DEPTH;	break;
	case kAirAbsorptionGainHF :    v = AirAbsorptionGainHF/EAXREVERB_MAX_AIR_ABSORPTION_GAINHF;	break;
	case kHFReference :    v = HFReference/EAXREVERB_MAX_HFREFERENCE;	break;
	case kLFReference :    v = LFReference/EAXREVERB_MAX_LFREFERENCE;	break;
	case kRoomRolloffFactor :    v = RoomRolloffFactor/EAXREVERB_MAX_ROOM_ROLLOFF_FACTOR;	break;
	case kDecayHFLimit :    v = DecayHFLimit;	break;
	}
	return v;
}


void EAXReverb::getParameterLabel (VstInt32 index, char *label)
{
	switch (index)
	{
	case kBalancelorig :    strcpy (label, "F");		break;
	case kBalancerorig :    strcpy (label, "F");		break;
	case kBalancelrev :    strcpy (label, "F");		break;
	case kBalancerrev :    strcpy (label, "F");		break;
	case kBalancel :    strcpy (label, "F");		break;
	case kBalancer :    strcpy (label, "F");		break;
	case kStereoorig :    strcpy (label, "F");		break;
	case kStereorev :    strcpy (label, "F");		break;
	case kStereo :    strcpy (label, "F");		break;
	case kDgain :    strcpy (label, "F");		break;
	case kWgain :    strcpy (label, "F");		break;
	case kMgain :    strcpy (label, "F");		break;
	case kRsmrate :    strcpy (label, "X");		break;
	case kBitdepth :    strcpy (label, "Bits");		break;
	case kLimit :    strcpy (label, "I");		break;
	case kFltfreq :    strcpy (label, "Hz");		break;
	case kFltres :    strcpy (label, "Db");		break;
	case kFltgain :    strcpy (label, "Db");		break;
	case kRate :    strcpy (label, "Hz");		break;
	case kDensity :    strcpy (label, "F");		break;
	case kDiffusion :    strcpy (label, "F");		break;
	case kGain :    strcpy (label, "F");		break;
	case kGainHF :    strcpy (label, "F");		break;
	case kGainLF :    strcpy (label, "F");		break;
	case kDecayTime :    strcpy (label, "Seconds");		break;
	case kDecayHFRatio :    strcpy (label, "F");		break;
	case kDecayLFRatio :    strcpy (label, "F");		break;
	case kReflectionsGain :    strcpy (label, "F");		break;
	case kReflectionsDelay :    strcpy (label, "Seconds");		break;
	case kReflectionsPanX :    strcpy (label, "F");		break;
	case kReflectionsPanY :    strcpy (label, "F");		break;
	case kReflectionsPanZ :    strcpy (label, "F");		break;
	case kLateReverbGain :    strcpy (label, "F");		break;
	case kLateReverbDelay :    strcpy (label, "Seconds");		break;
	case kLateReverbPanX :    strcpy (label, "F");		break;
	case kLateReverbPanY :    strcpy (label, "F");		break;
	case kLateReverbPanZ :    strcpy (label, "F");		break;
	case kEchoTime :    strcpy (label, "Seconds");		break;
	case kEchoDepth :    strcpy (label, "F");		break;
	case kModulationTime :    strcpy (label, "Seconds");		break;
	case kModulationDepth :    strcpy (label, "F");		break;
	case kAirAbsorptionGainHF :    strcpy (label, "F");		break;
	case kHFReference :    strcpy (label, "Hz");		break;
	case kLFReference :    strcpy (label, "Hz");		break;
	case kRoomRolloffFactor :    strcpy (label, "F");		break;
	}
}


void EAXReverb::getParameterName (VstInt32 index, char *text)
{
	switch (index)
	{
	case kDisable :    strcpy (text, "DisableEffect");		break;
	case kMute :    strcpy (text, "MuteEffect");		break;
	case kInvertmode :    strcpy (text, "InvertMode");		break;
	case kMonomode :    strcpy (text, "MonoMode");		break;
	case kInvertorig :    strcpy (text, "InvertOriginal");		break;
	case kInvertrev :    strcpy (text, "InvertReverb");		break;
	case kInvert :    strcpy (text, "Invert");		break;
	case kSwaporig :    strcpy (text, "SwapOriginal");		break;
	case kSwaprev :    strcpy (text, "SwapReverb");		break;
	case kSwap :    strcpy (text, "Swap");		break;
	case kBalancelorig :    strcpy (text, "BalanceLOriginal");		break;
	case kBalancerorig :    strcpy (text, "BalanceROriginal");		break;
	case kBalancelrev :    strcpy (text, "BalanceLReverb");		break;
	case kBalancerrev :    strcpy (text, "BalanceRReverb");		break;
	case kBalancel :    strcpy (text, "BalanceL");		break;
	case kBalancer :    strcpy (text, "BalanceR");		break;
	case kStereoorig :    strcpy (text, "StereoWidthOriginal");		break;
	case kStereorev :    strcpy (text, "StereoWidthReverb");		break;
	case kStereo :    strcpy (text, "StereoWidth");		break;
	case kMonoorig :    strcpy (text, "MonoOriginal");		break;
	case kMonorev :    strcpy (text, "MonoReverb");		break;
	case kMono :    strcpy (text, "Mono");		break;
	case kOnlyorig :    strcpy (text, "OnlyOriginal");		break;
	case kOnlyrev :    strcpy (text, "OnlyReverb");		break;
	case kMix :    strcpy (text, "MixMode");		break;
	case kDgain :    strcpy (text, "DryGain");		break;
	case kWgain :    strcpy (text, "WetGain");		break;
	case kMgain :    strcpy (text, "MasterGain");		break;
	case kResample :    strcpy (text, "Resample");		break;
	case kRsmq :    strcpy (text, "ResampleQuality");		break;
	case kRsmmode :    strcpy (text, "ResampleMode");		break;
	case kRsmrate :    strcpy (text, "ResampleRate");		break;
	case kBitcrush :    strcpy (text, "BitCrush");		break;
	case kBitdepth :    strcpy (text, "BitDepth");		break;
	case kDither :    strcpy (text, "Dither");		break;
	case kAutodither :    strcpy (text, "AutoDither");		break;
	case kOnlyqerror :    strcpy (text, "OnlyQuantizationError ");		break;
	case kLimit :    strcpy (text, "LimitSampleValues");		break;
	case kBqfilter :    strcpy (text, "BiquadFilter");		break;
	case kFlttype :    strcpy (text, "FilterType");		break;
	case kFltfreq :    strcpy (text, "FilterFrequency");		break;
	case kFltres :    strcpy (text, "FilterResonance");		break;
	case kFltgain :    strcpy (text, "FilterGain");		break;
	case kIncorrect :    strcpy (text, "IncorrectMode");		break;
	case kRate :    strcpy (text, "ReverbRate");		break;
	case kPreset :    strcpy (text, "ReverbPreset");		break;
	case kDensity :    strcpy (text, "Density");		break;
	case kDiffusion :    strcpy (text, "Diffusion");		break;
	case kGain :    strcpy (text, "Gain");		break;
	case kGainHF :    strcpy (text, "GainHF");		break;
	case kGainLF :    strcpy (text, "GainLF");		break;
	case kDecayTime :    strcpy (text, "DecayTime");		break;
	case kDecayHFRatio :    strcpy (text, "DecayHFRatio");		break;
	case kDecayLFRatio :    strcpy (text, "DecayLFRatio");		break;
	case kReflectionsGain :    strcpy (text, "ReflectionsGain");		break;
	case kReflectionsDelay :    strcpy (text, "ReflectionsDelay");		break;
	case kReflectionsPanX :    strcpy (text, "ReflectionsPanX");		break;
	case kReflectionsPanY :    strcpy (text, "ReflectionsPanY");		break;
	case kReflectionsPanZ :    strcpy (text, "ReflectionsPanZ");		break;
	case kLateReverbGain :    strcpy (text, "LateReverbGain");		break;
	case kLateReverbDelay :    strcpy (text, "LateReverbDelay");		break;
	case kLateReverbPanX :    strcpy (text, "LateReverbPanX");		break;
	case kLateReverbPanY :    strcpy (text, "LateReverbPanY");		break;
	case kLateReverbPanZ :    strcpy (text, "LateReverbPanZ");		break;
	case kEchoTime :    strcpy (text, "EchoTime");		break;
	case kEchoDepth :    strcpy (text, "EchoDepth");		break;
	case kModulationTime :    strcpy (text, "ModulationTime");		break;
	case kModulationDepth :    strcpy (text, "ModulationDepth");		break;
	case kAirAbsorptionGainHF :    strcpy (text, "AirAbsorptionGainHF");		break;
	case kHFReference :    strcpy (text, "HFReference");		break;
	case kLFReference :    strcpy (text, "LFReference");		break;
	case kRoomRolloffFactor :    strcpy (text, "RoomRolloffFactor");		break;
	case kDecayHFLimit :    strcpy (text, "DecayHFLimit");		break;
	}
}


void EAXReverb::getParameterDisplay (VstInt32 index, char *text)
{
	switch (index)
	{
	case kDisable :
		if (DisableEffect >= 0.5)	
		{
			strcpy (text, "ON");					
		}
		else
		{
			strcpy (text, "OFF");					
		}
		break;
	case kMute :
		if (MuteEffect >= 0.5)	
		{
			strcpy (text, "ON");					
		}
		else
		{
			strcpy (text, "OFF");					
		}
		break;
	case kInvertmode :
		if (InvertMode >= 0.25 && InvertMode < 0.5)	
		{
			strcpy (text, "LEFT");					
		}
		else if (InvertMode >= 0.5 && InvertMode <= 1.0)	
		{
			strcpy (text, "RIGHT");					
		}
		else
		{
			strcpy (text, "BOTH");					
		}
		break;
	case kMonomode :
		if (MonoMode >= 0.25 && MonoMode < 0.5)	
		{
			strcpy (text, "LEFT");					
		}
		else if (MonoMode >= 0.5 && MonoMode <= 1.0)	
		{
			strcpy (text, "RIGHT");					
		}
		else
		{
			strcpy (text, "BOTH");					
		}
		break;
	case kInvertorig :
		if (InvertOriginal >= 0.5)	
		{
			strcpy (text, "ON");					
		}
		else
		{
			strcpy (text, "OFF");					
		}
		break;
	case kInvertrev :
		if (InvertReverb >= 0.5)	
		{
			strcpy (text, "ON");					
		}
		else
		{
			strcpy (text, "OFF");					
		}
		break;
	case kInvert :
		if (Invert >= 0.5)	
		{
			strcpy (text, "ON");					
		}
		else
		{
			strcpy (text, "OFF");					
		}
		break;
	case kSwaporig :
		if (SwapOriginal >= 0.5)	
		{
			strcpy (text, "ON");					
		}
		else
		{
			strcpy (text, "OFF");					
		}
		break;
	case kSwaprev :
		if (SwapReverb >= 0.5)	
		{
			strcpy (text, "ON");					
		}
		else
		{
			strcpy (text, "OFF");					
		}
		break;
	case kSwap :
		if (Swap >= 0.5)	
		{
			strcpy (text, "ON");					
		}
		else
		{
			strcpy (text, "OFF");					
		}
		break;
	case kBalancelorig : float2string (BalanceLOriginal, text, kVstMaxParamStrLen);	break;
	case kBalancerorig : float2string (BalanceROriginal, text, kVstMaxParamStrLen);	break;
	case kBalancelrev : float2string (BalanceLReverb, text, kVstMaxParamStrLen);	break;
	case kBalancerrev : float2string (BalanceRReverb, text, kVstMaxParamStrLen);	break;
	case kBalancel : float2string (BalanceL, text, kVstMaxParamStrLen);	break;
	case kBalancer : float2string (BalanceR, text, kVstMaxParamStrLen);	break;
	case kStereoorig : float2string (StereoWidthOriginal, text, kVstMaxParamStrLen);	break;
	case kStereorev : float2string (StereoWidthReverb, text, kVstMaxParamStrLen);	break;
	case kStereo : float2string (StereoWidth, text, kVstMaxParamStrLen);	break;
	case kMonoorig :
		if (MonoOriginal >= 0.5)	
		{
			strcpy (text, "ON");					
		}
		else
		{
			strcpy (text, "OFF");					
		}
		break;
	case kMonorev :
		if (MonoReverb >= 0.5)	
		{
			strcpy (text, "ON");					
		}
		else
		{
			strcpy (text, "OFF");					
		}
		break;
	case kMono :
		if (Mono >= 0.5)	
		{
			strcpy (text, "ON");					
		}
		else
		{
			strcpy (text, "OFF");					
		}
		break;
	case kOnlyorig :
		if (OnlyOriginal >= 0.5)	
		{
			strcpy (text, "ON");					
		}
		else
		{
			strcpy (text, "OFF");					
		}
		break;
	case kOnlyrev :
		if (OnlyReverb >= 0.5)	
		{
			strcpy (text, "ON");					
		}
		else
		{
			strcpy (text, "OFF");					
		}
		break;
	case kMix :
		if (MixMode >= 0.5)	
		{
			strcpy (text, "MULTIPLY");					
		}
		else
		{
			strcpy (text, "ADD");					
		}
		break;
	case kDgain : float2string (DryGain, text, kVstMaxParamStrLen);	break;
	case kWgain : float2string (WetGain, text, kVstMaxParamStrLen);	break;
	case kMgain : float2string (MasterGain, text, kVstMaxParamStrLen);	break;
	case kResample :
		if (Resample >= 0.5)	
		{
			strcpy (text, "ON");					
		}
		else
		{
			strcpy (text, "OFF");					
		}
		break;
	case kRsmq :
		if (RSMQ >= 0.5)	
		{
			strcpy (text, "SINC");					
		}
		else if (RSMQ >= 0.25 && RSMQ < 0.5)	
		{
			strcpy (text, "ZOH");					
		}
		else
		{
			strcpy (text, "LINEAR");					
		}
		break;
	case kRsmmode :
		if (RSMMode >= 0.5)	
		{
			strcpy (text, "UP");					
		}
		else
		{
			strcpy (text, "DOWN");					
		}
		break;
	case kRsmrate : int2string (rsm, text, kVstMaxParamStrLen);	break;
	case kBitcrush :
		if (BitCr >= 0.5)	
		{
			strcpy (text, "ON");					
		}
		else
		{
			strcpy (text, "OFF");					
		}
		break;
	case kBitdepth : int2string (bits, text, kVstMaxParamStrLen);	break;
	case kDither :
		if (Dither >= 0.25 && Dither < 0.5)	
		{
			strcpy (text, "RECTANGLE");					
		}
		else if (Dither >= 0.5 && Dither < 0.75)	
		{
			strcpy (text, "TRIANGLE");					
		}
		else if (Dither >= 0.75 && Dither <= 1.0)	
		{
			strcpy (text, "GAUSSIAN");					
		}
		else
		{
			strcpy (text, "NONE");					
		}
		break;
	case kAutodither :
		if (AutoDither >= 0.5)	
		{
			strcpy (text, "ON");					
		}
		else
		{
			strcpy (text, "OFF");					
		}
		break;
	case kOnlyqerror :
		if (OnlyQError >= 0.5)	
		{
			strcpy (text, "ON");					
		}
		else
		{
			strcpy (text, "OFF");					
		}
		break;
	case kLimit : int2string (int(Limit*32767.0f), text, kVstMaxParamStrLen);	break;
	case kBqfilter :
		if (BQFilter >= 0.5)	
		{
			strcpy (text, "ON");					
		}
		else
		{
			strcpy (text, "OFF");					
		}
		break;
	case kFlttype :
		if (FLTType >= 0.0 && FLTType < 0.125)	
		{
			strcpy (text, "LOWPASS");					
		}
		else if (FLTType >= 0.125 && FLTType < 0.25)	
		{
			strcpy (text, "HIGHPASS");					
		}
		else if (FLTType >= 0.25 && FLTType < 0.375)	
		{
			strcpy (text, "BANDPASS");					
		}
		else if (FLTType >= 0.375 && FLTType < 0.5)	
		{
			strcpy (text, "ALLPASS");					
		}
		else if (FLTType >= 0.5 && FLTType < 0.625)	
		{
			strcpy (text, "NOTCH");					
		}
		else if (FLTType >= 0.625 && FLTType < 0.75)	
		{
			strcpy (text, "PEAKING");					
		}
		else if (FLTType >= 0.75 && FLTType < 0.875)	
		{
			strcpy (text, "LOWSHELF");					
		}
		else if (FLTType >= 0.875 && FLTType <= 1.0)	
		{
			strcpy (text, "HIGHSHELF");					
		}
		else
		{
			strcpy (text, "NONE");					
		}
		break;
	case kFltfreq : float2string (FLTFreq, text, kVstMaxParamStrLen);	break;
	case kFltres : float2string (FLTRes, text, kVstMaxParamStrLen);	break;
	case kFltgain : float2string (FLTGain, text, kVstMaxParamStrLen);	break;
	case kIncorrect :
		if (IncorrectMode >= 0.5)	
		{
			strcpy (text, "ON");					
		}
		else
		{
			strcpy (text, "OFF");					
		}
		break;
	case kRate : int2string (int(ReverbRate), text, kVstMaxParamStrLen);	break;
	case kPreset : strcpy (text, GetPresetName (i_ReverbPreset));	break;
	case kDensity : float2string (Density, text, kVstMaxParamStrLen);	break;
	case kDiffusion : float2string (Diffusion, text, kVstMaxParamStrLen);	break;
	case kGain : float2string (Gain, text, kVstMaxParamStrLen);	break;
	case kGainHF : float2string (GainHF, text, kVstMaxParamStrLen);	break;
	case kGainLF : float2string (GainLF, text, kVstMaxParamStrLen);	break;
	case kDecayTime : float2string (DecayTime, text, kVstMaxParamStrLen);	break;
	case kDecayHFRatio : float2string (DecayHFRatio, text, kVstMaxParamStrLen);	break;
	case kDecayLFRatio : float2string (DecayLFRatio, text, kVstMaxParamStrLen);	break;
	case kReflectionsGain : float2string (ReflectionsGain, text, kVstMaxParamStrLen);	break;
	case kReflectionsDelay : float2string (ReflectionsDelay, text, kVstMaxParamStrLen);	break;
	case kReflectionsPanX : float2string (ReflectionsPanX, text, kVstMaxParamStrLen);	break;
	case kReflectionsPanY : float2string (ReflectionsPanY, text, kVstMaxParamStrLen);	break;
	case kReflectionsPanZ : float2string (ReflectionsPanZ, text, kVstMaxParamStrLen);	break;
	case kLateReverbGain : float2string (LateReverbGain, text, kVstMaxParamStrLen);	break;
	case kLateReverbDelay : float2string (LateReverbDelay, text, kVstMaxParamStrLen);	break;
	case kLateReverbPanX : float2string (LateReverbPanX, text, kVstMaxParamStrLen);	break;
	case kLateReverbPanY : float2string (LateReverbPanY, text, kVstMaxParamStrLen);	break;
	case kLateReverbPanZ : float2string (LateReverbPanZ, text, kVstMaxParamStrLen);	break;
	case kEchoTime : float2string (EchoTime, text, kVstMaxParamStrLen);	break;
	case kEchoDepth : float2string (EchoDepth, text, kVstMaxParamStrLen);	break;
	case kModulationTime : float2string (ModulationTime, text, kVstMaxParamStrLen);	break;
	case kModulationDepth : float2string (ModulationDepth, text, kVstMaxParamStrLen);	break;
	case kAirAbsorptionGainHF : float2string (AirAbsorptionGainHF, text, kVstMaxParamStrLen);	break;
	case kHFReference : float2string (HFReference, text, kVstMaxParamStrLen);	break;
	case kLFReference : float2string (LFReference, text, kVstMaxParamStrLen);	break;
	case kRoomRolloffFactor : float2string (RoomRolloffFactor, text, kVstMaxParamStrLen);	break;
	case kDecayHFLimit :
		if (DecayHFLimit >= 0.5)	
		{
			strcpy (text, "ON");					
		}
		else
		{
			strcpy (text, "OFF");					
		}
		break;
	}
}


VstInt32 EAXReverb::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know


bool EAXReverb::getEffectName (char* name)
{
	strcpy (name, "EAXReverb");
	return true;
}


bool EAXReverb::getProductString (char* text)
{
	strcpy (text, "EAXReverb");
	return true;
}


bool EAXReverb::getVendorString (char* text)
{
	strcpy (text, "Datajake");
	return true;
}


void EAXReverb::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	float* in1 = inputs[0];
	float* in2 = inputs[1];
	float* out1 = outputs[0];
	float* out2 = outputs[1];
	int i;
	//check if we are disabling the effect
	if (DisableEffect >= 0.5)
	{
		for (i=0; i<sampleFrames; i++)
		{
			*out1 = *in1;
			*out2 = *in2;
			*in1++;
			*in2++;
			*out1++;
			*out2++;
		}
		return;
	}
	//check if we are muting the effect
	if (MuteEffect >= 0.5)
	{
		for (i=0; i<sampleFrames; i++)
		{
			*out1 = 0;
			*out2 = 0;
			*out1++;
			*out2++;
		}
		return;
	}
	//allocate memory for mono samples
	float *floatSamplesIn =  new float[sampleFrames];
	//convert stereo samples into mono
	for (i=0; i<sampleFrames; i++)
	{
		floatSamplesIn[i] = (*in1 + *in2) / 2;
		*in1++;
		*in2++;
	}
	in1 -= sampleFrames;
	in2 -= sampleFrames;
	//set the offset for the audio buffer
	int offset = 0;
	//allocate memory for reverb output samples
	float floatSamplesOut[REVERB_BUFFERSIZE * OUTPUT_CHANNELS];
	do {
		//set the amount of samples to process at a time
		int workSamples = REVERB_BUFFERSIZE / 4;
		if (workSamples>sampleFrames)
		{
			workSamples = sampleFrames;
		}
		//process the effect
		effect.Process(workSamples, &floatSamplesIn[offset],  floatSamplesOut);
		//invert the phase of the original signal if we set InvertOriginal to true
		if (InvertOriginal >= 0.5)
		{
			for (i=0; i<workSamples; i++)
			{
				if (InvertMode >= 0.25 && InvertMode < 0.5)	
				{
					*in1 = *in1 * -1;
				}
				else if (InvertMode >= 0.5 && InvertMode <= 1.0)	
				{
					*in2 = *in2 * -1;
				}
				else
				{
					*in1 = *in1 * -1;
					*in2 = *in2 * -1;
				}
				*in1++;
				*in2++;
			}
			in1 -= workSamples;
			in2 -= workSamples;
		}
		//invert the phase of the reverb if we set InvertReverb to true
		if (InvertReverb >= 0.5)
		{
			for (i=0; i<workSamples; i++)
			{
				if (InvertMode >= 0.25 && InvertMode < 0.5)	
				{
					floatSamplesOut[i*2 + 0] = floatSamplesOut[i*2 + 0] * -1;
				}
				else if (InvertMode >= 0.5 && InvertMode <= 1.0)	
				{
					floatSamplesOut[i*2 + 1] = floatSamplesOut[i*2 + 1] * -1;
				}
				else
				{
					floatSamplesOut[i*2 + 0] = floatSamplesOut[i*2 + 0] * -1;
					floatSamplesOut[i*2 + 1] = floatSamplesOut[i*2 + 1] * -1;
				}
			}
		}
		//swap the channels of the original signal if we set SwapOriginal to true
		if (SwapOriginal >= 0.5)
		{
			float swap[2];
			for (i=0; i<workSamples; i++)
			{
				swap[0] = *in2;
				swap[1] = *in1;
				*in1 = swap[0];
				*in2 = swap[1];
				*in1++;
				*in2++;
			}
			in1 -= workSamples;
			in2 -= workSamples;
		}
		//swap the channels of the reverb if we set SwapReverb to true
		if (SwapReverb >= 0.5)
		{
			float swap[2];
			for (i=0; i<workSamples; i++)
			{
				swap[0] = floatSamplesOut[i*2 + 1];
				swap[1] = floatSamplesOut[i*2 + 0];
				floatSamplesOut[i*2 + 0] = swap[0];
				floatSamplesOut[i*2 + 1] = swap[1];
			}
		}
		//adjust balance of dry samples
		for (i=0; i<workSamples; i++)
		{
			*in1 = *in1 * BalanceLOriginal;
			*in2 = *in2 * BalanceROriginal;
			*in1++;
			*in2++;
		}
		in1 -= workSamples;
		in2 -= workSamples;
		//adjust balance of wet samples
		for (i=0; i<workSamples; i++)
		{
			floatSamplesOut[i*2 + 0] = floatSamplesOut[i*2 + 0] * BalanceLReverb;
			floatSamplesOut[i*2 + 1] = floatSamplesOut[i*2 + 1] * BalanceRReverb;
		}
		//adjust the stereo width of dry samples
		float Temp = 1/(1.0f + StereoWidthOriginal);
		float CoefficientM = 1.0f * Temp;
		float CoefficientS = StereoWidthOriginal * 0.5f;
		float ValueM = 0.0;
		float ValueS = 0.0;
		for(i = 0; i < workSamples; i++)
		{
			ValueM = (*in1 + *in2) * CoefficientM;
			ValueS = (*in2 - *in1) * CoefficientS;
			*in1 = ValueM - ValueS;
			*in2 = ValueM + ValueS;
			*in1++;
			*in2++;
		}
		in1 -= workSamples;
		in2 -= workSamples;
		//adjust the stereo width of wet samples
		Temp = 1/(1.0f + StereoWidthReverb);
		CoefficientM = 1.0f * Temp;
		CoefficientS = StereoWidthReverb * 0.5f;
		ValueM = 0.0;
		ValueS = 0.0;
		for(i = 0; i < workSamples; i++)
		{
			ValueM = (floatSamplesOut[i*2 + 0] + floatSamplesOut[i*2 + 1]) * CoefficientM;
			ValueS = (floatSamplesOut[i*2 + 1] - floatSamplesOut[i*2 + 0]) * CoefficientS;
			floatSamplesOut[i*2 + 0] = ValueM - ValueS;
			floatSamplesOut[i*2 + 1] = ValueM + ValueS;
		}
		//mixdown the original signal to mono if we set MonoOriginal to true
		if (MonoOriginal >= 0.5)
		{
			for (i=0; i<workSamples; i++)
			{
				float sample;
				if (MonoMode >= 0.25 && MonoMode < 0.5)	
				{
					sample = *in1;
				}
				else if (MonoMode >= 0.5 && MonoMode <= 1.0)	
				{
					sample = *in2;
				}
				else
				{
					sample = (*in1 + *in2) / 2;
				}
				*in1 = sample;
				*in2 = sample;
				*in1++;
				*in2++;
			}
			in1 -= workSamples;
			in2 -= workSamples;
		}
		//mixdown the reverb to mono if we set MonoReverb to true
		if (MonoReverb >= 0.5)
		{
			for (i=0; i<workSamples; i++)
			{
				float sample;
				if (MonoMode >= 0.25 && MonoMode < 0.5)	
				{
					sample = floatSamplesOut[i*2 + 0];
				}
				else if (MonoMode >= 0.5 && MonoMode <= 1.0)	
				{
					sample = floatSamplesOut[i*2 + 1];
				}
				else
				{
					sample = (floatSamplesOut[i*2 + 0] + floatSamplesOut[i*2 + 1]) / 2;
				}
				floatSamplesOut[i*2 + 0] = sample;
				floatSamplesOut[i*2 + 1] = sample;
			}
		}
		//apply gain to dry samples
		for (i=0; i<workSamples; i++)
		{
			*in1 = *in1 * DryGain;
			*in2 = *in2 * DryGain;
			*in1++;
			*in2++;
		}
		in1 -= workSamples;
		in2 -= workSamples;
		//apply gain to wet samples
		for (i=0; i<workSamples; i++)
		{
			floatSamplesOut[i*2 + 0] = floatSamplesOut[i*2 + 0] * WetGain;
			floatSamplesOut[i*2 + 1] = floatSamplesOut[i*2 + 1] * WetGain;
		}
		//write to the audio buffer
		for (i=0; i<workSamples; i++)
		{
			//check if we are only generating the original signal, the reverb output, or both
			if (OnlyOriginal >= 0.5)
			{
				*out1 = *in1;
				*out2 = *in2;
			}
			else if (OnlyReverb >= 0.5)
			{
				*out1 = floatSamplesOut[i*2 + 0];
				*out2 = floatSamplesOut[i*2 + 1];
			}
			else
			{
				//check if we are adding or multiplying the dry and wet signals
				if (MixMode >= 0.5)
				{
					*out1 = *in1 * floatSamplesOut[i*2 + 0];
					*out2 = *in2 * floatSamplesOut[i*2 + 1];
				}
				else
				{
					*out1 = *in1 + floatSamplesOut[i*2 + 0];
					*out2 = *in2 + floatSamplesOut[i*2 + 1];
				}
			}
			*in1++;
			*in2++;
			*out1++;
			*out2++;
		}
		out1 -= workSamples;
		out2 -= workSamples;
		//process a Biquad filter on the final output if we set BQFilter to true
		if (BQFilter >= 0.5)
		{
			sf_sample_st *buf =  new sf_sample_st[workSamples];
			for (i=0; i<workSamples; i++)
			{
				buf[i].L = *out1;
				buf[i].R = *out2;
				*out1++;
				*out2++;
			}
			out1 -= workSamples;
			out2 -= workSamples;
			sf_biquad_process(&bq_state, workSamples, buf, buf);
			for (i=0; i<workSamples; i++)
			{
				*out1 = buf[i].L;
				*out2 = buf[i].R;
				*out1++;
				*out2++;
			}
			out1 -= workSamples;
			out2 -= workSamples;
			delete[] buf;
		}
		//process the resampler on the final output if we set Resample to true
		if (Resample >= 0.5)
		{
			short *samples = new short[workSamples*2];
			GenerateSilence(samples, workSamples);
			for (i=0; i<workSamples*2; i+=2)
			{
				long sample = (long) (*out1 * 32767.0f);
				if (sample > 32767)
				{
					sample = 32767;
				}
				else if (sample < -32768)
				{
					sample = -32768;
				}
				samples[i] = (short)sample;
				sample = (long) (*out2 * 32767.0f);
				if (sample > 32767)
				{
					sample = 32767;
				}
				else if (sample < -32768)
				{
					sample = -32768;
				}
				samples[i+1] = (short)sample;
				*out1++;
				*out2++;
			}
			out1 -= workSamples;
			out2 -= workSamples;
			int numsamples_new;
			if (RSMMode >= 0.5)
			{
				numsamples_new = workSamples*rsm;
			}
			else
			{
				numsamples_new = workSamples/rsm;
			}
			short *samples_new = new short[numsamples_new*2];
			GenerateSilence(samples_new, numsamples_new);
			if (RSMQ >= 0.5)
			{
				int j;
				int samplecount = 0;
				for(i = 0; i < numsamples_new*2; i+=2)
				{
					sample_t ls, rs;
					for(j = 0; j = resampler_get_min_fill(resampler1); j++)
					{
						if (samplecount >= workSamples*2)
						{
							samplecount = workSamples*2 - 2;
						}
						resampler_write_pair(resampler1, samples[samplecount], samples[samplecount + 1]);
						samplecount += 2;
					}
					resampler_peek_pair(resampler1, &ls, &rs);
					resampler_read_pair(resampler1, &ls, &rs);
					if ((ls + 0x8000) & 0xFFFF0000) ls = (ls >> 31) ^ 0x7FFF;
					if ((rs + 0x8000) & 0xFFFF0000) rs = (rs >> 31) ^ 0x7FFF;
					samples_new[i] = (short)ls;
					samples_new[i + 1] = (short)rs;
				}
				samplecount = 0;
				for(i = 0; i < workSamples*2; i+=2)
				{
					sample_t ls, rs;
					for(j = 0; j = resampler_get_min_fill(resampler2); j++)
					{
						if (samplecount >= numsamples_new*2)
						{
							samplecount = numsamples_new*2 - 2;
						}
						resampler_write_pair(resampler2, samples_new[samplecount], samples_new[samplecount + 1]);
						samplecount += 2;
					}
					resampler_peek_pair(resampler2, &ls, &rs);
					resampler_read_pair(resampler2, &ls, &rs);
					if ((ls + 0x8000) & 0xFFFF0000) ls = (ls >> 31) ^ 0x7FFF;
					if ((rs + 0x8000) & 0xFFFF0000) rs = (rs >> 31) ^ 0x7FFF;
					samples[i] = (short)ls;
					samples[i + 1] = (short)rs;
				}
			}
			else if (RSMQ >= 0.25 && RSMQ < 0.5)	
			{
				ZOHResamplerProcess(zohresampler1, samples, workSamples, samples_new, numsamples_new);
				ZOHResamplerProcess(zohresampler2, samples_new, numsamples_new, samples, workSamples);
			}
			else
			{
				LinearResamplerProcess(linearresampler1, samples, workSamples, samples_new, numsamples_new);
				LinearResamplerProcess(linearresampler2, samples_new, numsamples_new, samples, workSamples);
			}
			delete[] samples_new;
			for (i=0; i<workSamples*2; i+=2)
			{
				*out1 = float(samples[i] / 32767.0f);
				*out2 = float(samples[i+1] / 32767.0f);
				*out1++;
				*out2++;
			}
			out1 -= workSamples;
			out2 -= workSamples;
			delete[] samples;
		}
		//process the bit crusher on the final output if we set BitCr to true
		if (BitCr >= 0.5)
		{
			short *samples = new short[workSamples*2];
			GenerateSilence(samples, workSamples);
			for (i=0; i<workSamples*2; i+=2)
			{
				long sample = (long) (*out1 * 32767.0f);
				if (sample > 32767)
				{
					sample = 32767;
				}
				else if (sample < -32768)
				{
					sample = -32768;
				}
				samples[i] = (short)sample;
				sample = (long) (*out2 * 32767.0f);
				if (sample > 32767)
				{
					sample = 32767;
				}
				else if (sample < -32768)
				{
					sample = -32768;
				}
				samples[i+1] = (short)sample;
				*out1++;
				*out2++;
			}
			out1 -= workSamples;
			out2 -= workSamples;
			if (Dither >= 0.25 && Dither < 0.5)	
			{
				RectangleDither(&bc, samples, workSamples);
			}
			else if (Dither >= 0.5 && Dither < 0.75)	
			{
				TriangleDither(&bc, samples, workSamples);
			}
			else if (Dither >= 0.75 && Dither <= 1.0)	
			{
				GaussianDither(&bc, samples, workSamples);
			}
			BitCrush(&bc, samples, workSamples);
			LimitOutput(int(Limit*32767.0f), samples, workSamples);
			for (i=0; i<workSamples*2; i+=2)
			{
				*out1 = float(samples[i] / 32767.0f);
				*out2 = float(samples[i+1] / 32767.0f);
				*out1++;
				*out2++;
			}
			out1 -= workSamples;
			out2 -= workSamples;
			delete[] samples;
		}
		//invert the phase of the final output if we set Invert to true
		if (Invert >= 0.5)
		{
			for (i=0; i<workSamples; i++)
			{
				if (InvertMode >= 0.25 && InvertMode < 0.5)	
				{
					*out1 = *out1 * -1;
				}
				else if (InvertMode >= 0.5 && InvertMode <= 1.0)	
				{
					*out2 = *out2 * -1;
				}
				else
				{
					*out1 = *out1 * -1;
					*out2 = *out2 * -1;
				}
				*out1++;
				*out2++;
			}
			out1 -= workSamples;
			out2 -= workSamples;
		}
		//swap the channels of the final output if we set Swap to true
		if (Swap >= 0.5)
		{
			float swap[2];
			for (i=0; i<workSamples; i++)
			{
				swap[0] = *out2;
				swap[1] = *out1;
				*out1 = swap[0];
				*out2 = swap[1];
				*out1++;
				*out2++;
			}
			out1 -= workSamples;
			out2 -= workSamples;
		}
		//adjust balance of final output
		for (i=0; i<workSamples; i++)
		{
			*out1 = *out1 * BalanceL;
			*out2 = *out2 * BalanceR;
			*out1++;
			*out2++;
		}
		out1 -= workSamples;
		out2 -= workSamples;
		//adjust the stereo width of final output
		Temp = 1/(1.0f + StereoWidth);
		CoefficientM = 1.0f * Temp;
		CoefficientS = StereoWidth * 0.5f;
		ValueM = 0.0;
		ValueS = 0.0;
		for(i = 0; i < workSamples; i++)
		{
			ValueM = (*out1 + *out2) * CoefficientM;
			ValueS = (*out2 - *out1) * CoefficientS;
			*out1 = ValueM - ValueS;
			*out2 = ValueM + ValueS;
			*out1++;
			*out2++;
		}
		out1 -= workSamples;
		out2 -= workSamples;
		//mixdown the final output to mono if we set Mono to true
		if (Mono >= 0.5)
		{
			for (i=0; i<workSamples; i++)
			{
				float sample;
				if (MonoMode >= 0.25 && MonoMode < 0.5)	
				{
					sample = *out1;
				}
				else if (MonoMode >= 0.5 && MonoMode <= 1.0)	
				{
					sample = *out2;
				}
				else
				{
					sample = (*out1 + *out2) / 2;
				}
				*out1 = sample;
				*out2 = sample;
				*out1++;
				*out2++;
			}
			out1 -= workSamples;
			out2 -= workSamples;
		}
		//apply gain to final output
		for (i=0; i<workSamples; i++)
		{
			*out1 = *out1 * MasterGain;
			*out2 = *out2 * MasterGain;
			*out1++;
			*out2++;
		}
		//update the sample counters
		sampleFrames -= workSamples;
		offset += workSamples;
	} while (sampleFrames>0);
	//delete the mono samples
	delete[] floatSamplesIn;
}


void EAXReverb::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames)
{
	double* in1 = inputs[0];
	double* in2 = inputs[1];
	double* out1 = outputs[0];
	double* out2 = outputs[1];
	int i;
	//check if we are disabling the effect
	if (DisableEffect >= 0.5)
	{
		for (i=0; i<sampleFrames; i++)
		{
			*out1 = *in1;
			*out2 = *in2;
			*in1++;
			*in2++;
			*out1++;
			*out2++;
		}
		return;
	}
	//check if we are muting the effect
	if (MuteEffect >= 0.5)
	{
		for (i=0; i<sampleFrames; i++)
		{
			*out1 = 0;
			*out2 = 0;
			*out1++;
			*out2++;
		}
		return;
	}
	//allocate memory for mono samples
	double *doubleSamplesIn =  new double[sampleFrames];
	//convert stereo samples into mono
	for (i=0; i<sampleFrames; i++)
	{
		doubleSamplesIn[i] = (*in1 + *in2) / 2;
		*in1++;
		*in2++;
	}
	in1 -= sampleFrames;
	in2 -= sampleFrames;
	//set the offset for the audio buffer
	int offset = 0;
	//allocate memory for reverb output samples
	double doubleSamplesOut[REVERB_BUFFERSIZE * OUTPUT_CHANNELS];
	do {
		//set the amount of samples to process at a time
		int workSamples = REVERB_BUFFERSIZE / 4;
		if (workSamples>sampleFrames)
		{
			workSamples = sampleFrames;
		}
		//process the effect
		effectDouble.Process(workSamples, &doubleSamplesIn[offset],  doubleSamplesOut);
		//invert the phase of the original signal if we set InvertOriginal to true
		if (InvertOriginal >= 0.5)
		{
			for (i=0; i<workSamples; i++)
			{
				if (InvertMode >= 0.25 && InvertMode < 0.5)	
				{
					*in1 = *in1 * -1;
				}
				else if (InvertMode >= 0.5 && InvertMode <= 1.0)	
				{
					*in2 = *in2 * -1;
				}
				else
				{
					*in1 = *in1 * -1;
					*in2 = *in2 * -1;
				}
				*in1++;
				*in2++;
			}
			in1 -= workSamples;
			in2 -= workSamples;
		}
		//invert the phase of the reverb if we set InvertReverb to true
		if (InvertReverb >= 0.5)
		{
			for (i=0; i<workSamples; i++)
			{
				if (InvertMode >= 0.25 && InvertMode < 0.5)	
				{
					doubleSamplesOut[i*2 + 0] = doubleSamplesOut[i*2 + 0] * -1;
				}
				else if (InvertMode >= 0.5 && InvertMode <= 1.0)	
				{
					doubleSamplesOut[i*2 + 1] = doubleSamplesOut[i*2 + 1] * -1;
				}
				else
				{
					doubleSamplesOut[i*2 + 0] = doubleSamplesOut[i*2 + 0] * -1;
					doubleSamplesOut[i*2 + 1] = doubleSamplesOut[i*2 + 1] * -1;
				}
			}
		}
		//swap the channels of the original signal if we set SwapOriginal to true
		if (SwapOriginal >= 0.5)
		{
			double swap[2];
			for (i=0; i<workSamples; i++)
			{
				swap[0] = *in2;
				swap[1] = *in1;
				*in1 = swap[0];
				*in2 = swap[1];
				*in1++;
				*in2++;
			}
			in1 -= workSamples;
			in2 -= workSamples;
		}
		//swap the channels of the reverb if we set SwapReverb to true
		if (SwapReverb >= 0.5)
		{
			double swap[2];
			for (i=0; i<workSamples; i++)
			{
				swap[0] = doubleSamplesOut[i*2 + 1];
				swap[1] = doubleSamplesOut[i*2 + 0];
				doubleSamplesOut[i*2 + 0] = swap[0];
				doubleSamplesOut[i*2 + 1] = swap[1];
			}
		}
		//adjust balance of dry samples
		for (i=0; i<workSamples; i++)
		{
			*in1 = *in1 * BalanceLOriginal;
			*in2 = *in2 * BalanceROriginal;
			*in1++;
			*in2++;
		}
		in1 -= workSamples;
		in2 -= workSamples;
		//adjust balance of wet samples
		for (i=0; i<workSamples; i++)
		{
			doubleSamplesOut[i*2 + 0] = doubleSamplesOut[i*2 + 0] * BalanceLReverb;
			doubleSamplesOut[i*2 + 1] = doubleSamplesOut[i*2 + 1] * BalanceRReverb;
		}
		//adjust the stereo width of dry samples
		double Temp = 1/(1.0f + StereoWidthOriginal);
		double CoefficientM = 1.0f * Temp;
		double CoefficientS = StereoWidthOriginal * 0.5f;
		double ValueM = 0.0;
		double ValueS = 0.0;
		for(i = 0; i < workSamples; i++)
		{
			ValueM = (*in1 + *in2) * CoefficientM;
			ValueS = (*in2 - *in1) * CoefficientS;
			*in1 = ValueM - ValueS;
			*in2 = ValueM + ValueS;
			*in1++;
			*in2++;
		}
		in1 -= workSamples;
		in2 -= workSamples;
		//adjust the stereo width of wet samples
		Temp = 1/(1.0f + StereoWidthReverb);
		CoefficientM = 1.0f * Temp;
		CoefficientS = StereoWidthReverb * 0.5f;
		ValueM = 0.0;
		ValueS = 0.0;
		for(i = 0; i < workSamples; i++)
		{
			ValueM = (doubleSamplesOut[i*2 + 0] + doubleSamplesOut[i*2 + 1]) * CoefficientM;
			ValueS = (doubleSamplesOut[i*2 + 1] - doubleSamplesOut[i*2 + 0]) * CoefficientS;
			doubleSamplesOut[i*2 + 0] = ValueM - ValueS;
			doubleSamplesOut[i*2 + 1] = ValueM + ValueS;
		}
		//mixdown the original signal to mono if we set MonoOriginal to true
		if (MonoOriginal >= 0.5)
		{
			for (i=0; i<workSamples; i++)
			{
				double sample;
				if (MonoMode >= 0.25 && MonoMode < 0.5)	
				{
					sample = *in1;
				}
				else if (MonoMode >= 0.5 && MonoMode <= 1.0)	
				{
					sample = *in2;
				}
				else
				{
					sample = (*in1 + *in2) / 2;
				}
				*in1 = sample;
				*in2 = sample;
				*in1++;
				*in2++;
			}
			in1 -= workSamples;
			in2 -= workSamples;
		}
		//mixdown the reverb to mono if we set MonoReverb to true
		if (MonoReverb >= 0.5)
		{
			for (i=0; i<workSamples; i++)
			{
				double sample;
				if (MonoMode >= 0.25 && MonoMode < 0.5)	
				{
					sample = doubleSamplesOut[i*2 + 0];
				}
				else if (MonoMode >= 0.5 && MonoMode <= 1.0)	
				{
					sample = doubleSamplesOut[i*2 + 1];
				}
				else
				{
					sample = (doubleSamplesOut[i*2 + 0] + doubleSamplesOut[i*2 + 1]) / 2;
				}
				doubleSamplesOut[i*2 + 0] = sample;
				doubleSamplesOut[i*2 + 1] = sample;
			}
		}
		//apply gain to dry samples
		for (i=0; i<workSamples; i++)
		{
			*in1 = *in1 * DryGain;
			*in2 = *in2 * DryGain;
			*in1++;
			*in2++;
		}
		in1 -= workSamples;
		in2 -= workSamples;
		//apply gain to wet samples
		for (i=0; i<workSamples; i++)
		{
			doubleSamplesOut[i*2 + 0] = doubleSamplesOut[i*2 + 0] * WetGain;
			doubleSamplesOut[i*2 + 1] = doubleSamplesOut[i*2 + 1] * WetGain;
		}
		//write to the audio buffer
		for (i=0; i<workSamples; i++)
		{
			//check if we are only generating the original signal, the reverb output, or both
			if (OnlyOriginal >= 0.5)
			{
				*out1 = *in1;
				*out2 = *in2;
			}
			else if (OnlyReverb >= 0.5)
			{
				*out1 = doubleSamplesOut[i*2 + 0];
				*out2 = doubleSamplesOut[i*2 + 1];
			}
			else
			{
				//check if we are adding or multiplying the dry and wet signals
				if (MixMode >= 0.5)
				{
					*out1 = *in1 * doubleSamplesOut[i*2 + 0];
					*out2 = *in2 * doubleSamplesOut[i*2 + 1];
				}
				else
				{
					*out1 = *in1 + doubleSamplesOut[i*2 + 0];
					*out2 = *in2 + doubleSamplesOut[i*2 + 1];
				}
			}
			*in1++;
			*in2++;
			*out1++;
			*out2++;
		}
		out1 -= workSamples;
		out2 -= workSamples;
		//process a Biquad filter on the final output if we set BQFilter to true
		if (BQFilter >= 0.5)
		{
			sfd_sample_st *buf =  new sfd_sample_st[workSamples];
			for (i=0; i<workSamples; i++)
			{
				buf[i].L = *out1;
				buf[i].R = *out2;
				*out1++;
				*out2++;
			}
			out1 -= workSamples;
			out2 -= workSamples;
			sfd_biquad_process(&bq_state_double, workSamples, buf, buf);
			for (i=0; i<workSamples; i++)
			{
				*out1 = buf[i].L;
				*out2 = buf[i].R;
				*out1++;
				*out2++;
			}
			out1 -= workSamples;
			out2 -= workSamples;
			delete[] buf;
		}
		//process the resampler on the final output if we set Resample to true
		if (Resample >= 0.5)
		{
			short *samples = new short[workSamples*2];
			GenerateSilence(samples, workSamples);
			for (i=0; i<workSamples*2; i+=2)
			{
				long sample = (long) (*out1 * 32767.0f);
				if (sample > 32767)
				{
					sample = 32767;
				}
				else if (sample < -32768)
				{
					sample = -32768;
				}
				samples[i] = (short)sample;
				sample = (long) (*out2 * 32767.0f);
				if (sample > 32767)
				{
					sample = 32767;
				}
				else if (sample < -32768)
				{
					sample = -32768;
				}
				samples[i+1] = (short)sample;
				*out1++;
				*out2++;
			}
			out1 -= workSamples;
			out2 -= workSamples;
			int numsamples_new;
			if (RSMMode >= 0.5)
			{
				numsamples_new = workSamples*rsm;
			}
			else
			{
				numsamples_new = workSamples/rsm;
			}
			short *samples_new = new short[numsamples_new*2];
			GenerateSilence(samples_new, numsamples_new);
			if (RSMQ >= 0.5)
			{
				int j;
				int samplecount = 0;
				for(i = 0; i < numsamples_new*2; i+=2)
				{
					sample_t ls, rs;
					for(j = 0; j = resampler_get_min_fill(resampler1); j++)
					{
						if (samplecount >= workSamples*2)
						{
							samplecount = workSamples*2 - 2;
						}
						resampler_write_pair(resampler1, samples[samplecount], samples[samplecount + 1]);
						samplecount += 2;
					}
					resampler_peek_pair(resampler1, &ls, &rs);
					resampler_read_pair(resampler1, &ls, &rs);
					if ((ls + 0x8000) & 0xFFFF0000) ls = (ls >> 31) ^ 0x7FFF;
					if ((rs + 0x8000) & 0xFFFF0000) rs = (rs >> 31) ^ 0x7FFF;
					samples_new[i] = (short)ls;
					samples_new[i + 1] = (short)rs;
				}
				samplecount = 0;
				for(i = 0; i < workSamples*2; i+=2)
				{
					sample_t ls, rs;
					for(j = 0; j = resampler_get_min_fill(resampler2); j++)
					{
						if (samplecount >= numsamples_new*2)
						{
							samplecount = numsamples_new*2 - 2;
						}
						resampler_write_pair(resampler2, samples_new[samplecount], samples_new[samplecount + 1]);
						samplecount += 2;
					}
					resampler_peek_pair(resampler2, &ls, &rs);
					resampler_read_pair(resampler2, &ls, &rs);
					if ((ls + 0x8000) & 0xFFFF0000) ls = (ls >> 31) ^ 0x7FFF;
					if ((rs + 0x8000) & 0xFFFF0000) rs = (rs >> 31) ^ 0x7FFF;
					samples[i] = (short)ls;
					samples[i + 1] = (short)rs;
				}
			}
			else if (RSMQ >= 0.25 && RSMQ < 0.5)	
			{
				ZOHResamplerProcess(zohresampler1, samples, workSamples, samples_new, numsamples_new);
				ZOHResamplerProcess(zohresampler2, samples_new, numsamples_new, samples, workSamples);
			}
			else
			{
				LinearResamplerProcess(linearresampler1, samples, workSamples, samples_new, numsamples_new);
				LinearResamplerProcess(linearresampler2, samples_new, numsamples_new, samples, workSamples);
			}
			delete[] samples_new;
			for (i=0; i<workSamples*2; i+=2)
			{
				*out1 = double(samples[i] / 32767.0f);
				*out2 = double(samples[i+1] / 32767.0f);
				*out1++;
				*out2++;
			}
			out1 -= workSamples;
			out2 -= workSamples;
			delete[] samples;
		}
		//process the bit crusher on the final output if we set BitCr to true
		if (BitCr >= 0.5)
		{
			short *samples = new short[workSamples*2];
			GenerateSilence(samples, workSamples);
			for (i=0; i<workSamples*2; i+=2)
			{
				long sample = (long) (*out1 * 32767.0f);
				if (sample > 32767)
				{
					sample = 32767;
				}
				else if (sample < -32768)
				{
					sample = -32768;
				}
				samples[i] = (short)sample;
				sample = (long) (*out2 * 32767.0f);
				if (sample > 32767)
				{
					sample = 32767;
				}
				else if (sample < -32768)
				{
					sample = -32768;
				}
				samples[i+1] = (short)sample;
				*out1++;
				*out2++;
			}
			out1 -= workSamples;
			out2 -= workSamples;
			if (Dither >= 0.25 && Dither < 0.5)	
			{
				RectangleDither(&bc, samples, workSamples);
			}
			else if (Dither >= 0.5 && Dither < 0.75)	
			{
				TriangleDither(&bc, samples, workSamples);
			}
			else if (Dither >= 0.75 && Dither <= 1.0)	
			{
				GaussianDither(&bc, samples, workSamples);
			}
			BitCrush(&bc, samples, workSamples);
			LimitOutput(int(Limit*32767.0f), samples, workSamples);
			for (i=0; i<workSamples*2; i+=2)
			{
				*out1 = double(samples[i] / 32767.0f);
				*out2 = double(samples[i+1] / 32767.0f);
				*out1++;
				*out2++;
			}
			out1 -= workSamples;
			out2 -= workSamples;
			delete[] samples;
		}
		//invert the phase of the final output if we set Invert to true
		if (Invert >= 0.5)
		{
			for (i=0; i<workSamples; i++)
			{
				if (InvertMode >= 0.25 && InvertMode < 0.5)	
				{
					*out1 = *out1 * -1;
				}
				else if (InvertMode >= 0.5 && InvertMode <= 1.0)	
				{
					*out2 = *out2 * -1;
				}
				else
				{
					*out1 = *out1 * -1;
					*out2 = *out2 * -1;
				}
				*out1++;
				*out2++;
			}
			out1 -= workSamples;
			out2 -= workSamples;
		}
		//swap the channels of the final output if we set Swap to true
		if (Swap >= 0.5)
		{
			double swap[2];
			for (i=0; i<workSamples; i++)
			{
				swap[0] = *out2;
				swap[1] = *out1;
				*out1 = swap[0];
				*out2 = swap[1];
				*out1++;
				*out2++;
			}
			out1 -= workSamples;
			out2 -= workSamples;
		}
		//adjust balance of final output
		for (i=0; i<workSamples; i++)
		{
			*out1 = *out1 * BalanceL;
			*out2 = *out2 * BalanceR;
			*out1++;
			*out2++;
		}
		out1 -= workSamples;
		out2 -= workSamples;
		//adjust the stereo width of final output
		Temp = 1/(1.0f + StereoWidth);
		CoefficientM = 1.0f * Temp;
		CoefficientS = StereoWidth * 0.5f;
		ValueM = 0.0;
		ValueS = 0.0;
		for(i = 0; i < workSamples; i++)
		{
			ValueM = (*out1 + *out2) * CoefficientM;
			ValueS = (*out2 - *out1) * CoefficientS;
			*out1 = ValueM - ValueS;
			*out2 = ValueM + ValueS;
			*out1++;
			*out2++;
		}
		out1 -= workSamples;
		out2 -= workSamples;
		//mixdown the final output to mono if we set Mono to true
		if (Mono >= 0.5)
		{
			for (i=0; i<workSamples; i++)
			{
				double sample;
				if (MonoMode >= 0.25 && MonoMode < 0.5)	
				{
					sample = *out1;
				}
				else if (MonoMode >= 0.5 && MonoMode <= 1.0)	
				{
					sample = *out2;
				}
				else
				{
					sample = (*out1 + *out2) / 2;
				}
				*out1 = sample;
				*out2 = sample;
				*out1++;
				*out2++;
			}
			out1 -= workSamples;
			out2 -= workSamples;
		}
		//apply gain to final output
		for (i=0; i<workSamples; i++)
		{
			*out1 = *out1 * MasterGain;
			*out2 = *out2 * MasterGain;
			*out1++;
			*out2++;
		}
		//update the sample counters
		sampleFrames -= workSamples;
		offset += workSamples;
	} while (sampleFrames>0);
	//delete the mono samples
	delete[] doubleSamplesIn;
}
