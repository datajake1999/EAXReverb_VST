#include "EAXReverb.h"

EAXReverb::EAXReverb (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, kNumPrograms, kNumParams)
{
	// init
	memset(&effect, 0, sizeof(effect));
	ReverbEffectCreate(&effect, (int)sampleRate);
	ReverbEffectUpdate(&effect, (int)sampleRate);
	floatSamplesIn = NULL;
	floatSamplesIn = new float[blockSize];
	if (floatSamplesIn)
	{
		memset(floatSamplesIn, 0, blockSize*sizeof(float));
	}
	for (VstInt32 i = 0; i < numPrograms; i++)
	{
		curProgram = i;
		SetReverbPreset (i);
		setProgramName (GetPresetName(i));
	}
	curProgram = 0;
	setNumInputs (2);// stereo input
	setNumOutputs (2);// stereo output
	setUniqueID ('EAX');
	canProcessReplacing ();
	DisableEffect = 0;
	DryGain = 1;
}

EAXReverb::~EAXReverb ()
{
	if (floatSamplesIn)
	{
		delete[] floatSamplesIn;
		floatSamplesIn = NULL;
	}
	ReverbEffectDestroy(&effect);
}

void EAXReverb::setParameter (VstInt32 index, float value)
{
	switch (index)
	{
	case kDisable:
		DisableEffect = value;
		break;
	case kDgain:
		DryGain = value;
		break;
	case kDensity:
		value = value*EAXREVERB_MAX_DENSITY;
		if (value > EAXREVERB_MAX_DENSITY)
		{
			value = EAXREVERB_MAX_DENSITY;
		}
		else if (value < EAXREVERB_MIN_DENSITY)
		{
			value = EAXREVERB_MIN_DENSITY;
		}
		programs[curProgram].properties.flDensity = value;
		break;
	case kDiffusion:
		value = value*EAXREVERB_MAX_DIFFUSION;
		if (value > EAXREVERB_MAX_DIFFUSION)
		{
			value = EAXREVERB_MAX_DIFFUSION;
		}
		else if (value < EAXREVERB_MIN_DIFFUSION)
		{
			value = EAXREVERB_MIN_DIFFUSION;
		}
		programs[curProgram].properties.flDiffusion = value;
		break;
	case kGain:
		value = value*EAXREVERB_MAX_GAIN;
		if (value > EAXREVERB_MAX_GAIN)
		{
			value = EAXREVERB_MAX_GAIN;
		}
		else if (value < EAXREVERB_MIN_GAIN)
		{
			value = EAXREVERB_MIN_GAIN;
		}
		programs[curProgram].properties.flGain = value;
		break;
	case kGainHF:
		value = value*EAXREVERB_MAX_GAINHF;
		if (value > EAXREVERB_MAX_GAINHF)
		{
			value = EAXREVERB_MAX_GAINHF;
		}
		else if (value < EAXREVERB_MIN_GAINHF)
		{
			value = EAXREVERB_MIN_GAINHF;
		}
		programs[curProgram].properties.flGainHF = value;
		break;
	case kGainLF:
		value = value*EAXREVERB_MAX_GAINLF;
		if (value > EAXREVERB_MAX_GAINLF)
		{
			value = EAXREVERB_MAX_GAINLF;
		}
		else if (value < EAXREVERB_MIN_GAINLF)
		{
			value = EAXREVERB_MIN_GAINLF;
		}
		programs[curProgram].properties.flGainLF = value;
		break;
	case kDecayTime:
		value = value*EAXREVERB_MAX_DECAY_TIME;
		if (value > EAXREVERB_MAX_DECAY_TIME)
		{
			value = EAXREVERB_MAX_DECAY_TIME;
		}
		else if (value < EAXREVERB_MIN_DECAY_TIME)
		{
			value = EAXREVERB_MIN_DECAY_TIME;
		}
		programs[curProgram].properties.flDecayTime = value;
		break;
	case kDecayHFRatio:
		value = value*EAXREVERB_MAX_DECAY_HFRATIO;
		if (value > EAXREVERB_MAX_DECAY_HFRATIO)
		{
			value = EAXREVERB_MAX_DECAY_HFRATIO;
		}
		else if (value < EAXREVERB_MIN_DECAY_HFRATIO)
		{
			value = EAXREVERB_MIN_DECAY_HFRATIO;
		}
		programs[curProgram].properties.flDecayHFRatio = value;
		break;
	case kDecayLFRatio:
		value = value*EAXREVERB_MAX_DECAY_LFRATIO;
		if (value > EAXREVERB_MAX_DECAY_LFRATIO)
		{
			value = EAXREVERB_MAX_DECAY_LFRATIO;
		}
		else if (value < EAXREVERB_MIN_DECAY_LFRATIO)
		{
			value = EAXREVERB_MIN_DECAY_LFRATIO;
		}
		programs[curProgram].properties.flDecayLFRatio = value;
		break;
	case kReflectionsGain:
		value = value*EAXREVERB_MAX_REFLECTIONS_GAIN;
		if (value > EAXREVERB_MAX_REFLECTIONS_GAIN)
		{
			value = EAXREVERB_MAX_REFLECTIONS_GAIN;
		}
		else if (value < EAXREVERB_MIN_REFLECTIONS_GAIN)
		{
			value = EAXREVERB_MIN_REFLECTIONS_GAIN;
		}
		programs[curProgram].properties.flReflectionsGain = value;
		break;
	case kReflectionsDelay:
		value = value*EAXREVERB_MAX_REFLECTIONS_DELAY;
		if (value > EAXREVERB_MAX_REFLECTIONS_DELAY)
		{
			value = EAXREVERB_MAX_REFLECTIONS_DELAY;
		}
		else if (value < EAXREVERB_MIN_REFLECTIONS_DELAY)
		{
			value = EAXREVERB_MIN_REFLECTIONS_DELAY;
		}
		programs[curProgram].properties.flReflectionsDelay = value;
		break;
	case kReflectionsPanX:
		programs[curProgram].properties.flReflectionsPan[0] = value;
		break;
	case kReflectionsPanY:
		programs[curProgram].properties.flReflectionsPan[1] = value;
		break;
	case kReflectionsPanZ:
		programs[curProgram].properties.flReflectionsPan[2] = value;
		break;
	case kLateReverbGain:
		value = value*EAXREVERB_MAX_LATE_REVERB_GAIN;
		if (value > EAXREVERB_MAX_LATE_REVERB_GAIN)
		{
			value = EAXREVERB_MAX_LATE_REVERB_GAIN;
		}
		else if (value < EAXREVERB_MIN_LATE_REVERB_GAIN)
		{
			value = EAXREVERB_MIN_LATE_REVERB_GAIN;
		}
		programs[curProgram].properties.flLateReverbGain = value;
		break;
	case kLateReverbDelay:
		value = value*EAXREVERB_MAX_LATE_REVERB_DELAY;
		if (value > EAXREVERB_MAX_LATE_REVERB_DELAY)
		{
			value = EAXREVERB_MAX_LATE_REVERB_DELAY;
		}
		else if (value < EAXREVERB_MIN_LATE_REVERB_DELAY)
		{
			value = EAXREVERB_MIN_LATE_REVERB_DELAY;
		}
		programs[curProgram].properties.flLateReverbDelay = value;
		break;
	case kLateReverbPanX:
		programs[curProgram].properties.flLateReverbPan[0] = value;
		break;
	case kLateReverbPanY:
		programs[curProgram].properties.flLateReverbPan[1] = value;
		break;
	case kLateReverbPanZ:
		programs[curProgram].properties.flLateReverbPan[2] = value;
		break;
	case kEchoTime:
		value = value*EAXREVERB_MAX_ECHO_TIME;
		if (value > EAXREVERB_MAX_ECHO_TIME)
		{
			value = EAXREVERB_MAX_ECHO_TIME;
		}
		else if (value < EAXREVERB_MIN_ECHO_TIME)
		{
			value = EAXREVERB_MIN_ECHO_TIME;
		}
		programs[curProgram].properties.flEchoTime = value;
		break;
	case kEchoDepth:
		value = value*EAXREVERB_MAX_ECHO_DEPTH;
		if (value > EAXREVERB_MAX_ECHO_DEPTH)
		{
			value = EAXREVERB_MAX_ECHO_DEPTH;
		}
		else if (value < EAXREVERB_MIN_ECHO_DEPTH)
		{
			value = EAXREVERB_MIN_ECHO_DEPTH;
		}
		programs[curProgram].properties.flEchoDepth = value;
		break;
	case kModulationTime:
		value = value*EAXREVERB_MAX_MODULATION_TIME;
		if (value > EAXREVERB_MAX_MODULATION_TIME)
		{
			value = EAXREVERB_MAX_MODULATION_TIME;
		}
		else if (value < EAXREVERB_MIN_MODULATION_TIME)
		{
			value = EAXREVERB_MIN_MODULATION_TIME;
		}
		programs[curProgram].properties.flModulationTime = value;
		break;
	case kModulationDepth:
		value = value*EAXREVERB_MAX_MODULATION_DEPTH;
		if (value > EAXREVERB_MAX_MODULATION_DEPTH)
		{
			value = EAXREVERB_MAX_MODULATION_DEPTH;
		}
		else if (value < EAXREVERB_MIN_MODULATION_DEPTH)
		{
			value = EAXREVERB_MIN_MODULATION_DEPTH;
		}
		programs[curProgram].properties.flModulationDepth = value;
		break;
	case kAirAbsorptionGainHF:
		value = value*EAXREVERB_MAX_AIR_ABSORPTION_GAINHF;
		if (value > EAXREVERB_MAX_AIR_ABSORPTION_GAINHF)
		{
			value = EAXREVERB_MAX_AIR_ABSORPTION_GAINHF;
		}
		else if (value < EAXREVERB_MIN_AIR_ABSORPTION_GAINHF)
		{
			value = EAXREVERB_MIN_AIR_ABSORPTION_GAINHF;
		}
		programs[curProgram].properties.flAirAbsorptionGainHF = value;
		break;
	case kHFReference:
		value = value*EAXREVERB_MAX_HFREFERENCE;
		if (value > EAXREVERB_MAX_HFREFERENCE)
		{
			value = EAXREVERB_MAX_HFREFERENCE;
		}
		else if (value < EAXREVERB_MIN_HFREFERENCE)
		{
			value = EAXREVERB_MIN_HFREFERENCE;
		}
		programs[curProgram].properties.flHFReference = value;
		break;
	case kLFReference:
		value = value*EAXREVERB_MAX_LFREFERENCE;
		if (value > EAXREVERB_MAX_LFREFERENCE)
		{
			value = EAXREVERB_MAX_LFREFERENCE;
		}
		else if (value < EAXREVERB_MIN_LFREFERENCE)
		{
			value = EAXREVERB_MIN_LFREFERENCE;
		}
		programs[curProgram].properties.flLFReference = value;
		break;
	case kRoomRolloffFactor:
		value = value*EAXREVERB_MAX_ROOM_ROLLOFF_FACTOR;
		if (value > EAXREVERB_MAX_ROOM_ROLLOFF_FACTOR)
		{
			value = EAXREVERB_MAX_ROOM_ROLLOFF_FACTOR;
		}
		else if (value < EAXREVERB_MIN_ROOM_ROLLOFF_FACTOR)
		{
			value = EAXREVERB_MIN_ROOM_ROLLOFF_FACTOR;
		}
		programs[curProgram].properties.flRoomRolloffFactor = value;
		break;
	case kDecayHFLimit:
		value = value*EAXREVERB_MAX_DECAY_HFLIMIT;
		if (value > EAXREVERB_MAX_DECAY_HFLIMIT)
		{
			value = EAXREVERB_MAX_DECAY_HFLIMIT;
		}
		else if (value < EAXREVERB_MIN_DECAY_HFLIMIT)
		{
			value = EAXREVERB_MIN_DECAY_HFLIMIT;
		}
		programs[curProgram].properties.iDecayHFLimit = (int)value;
		break;
	}
	if (index >= kDensity && index <= kDecayHFLimit)
	{
		UpdateEffect();
	}
}

float EAXReverb::getParameter (VstInt32 index)
{
	EAXReverbProgram* ap = &programs[curProgram];
	float v = 0;
	switch (index)
	{
	case kDisable:
		v = DisableEffect;
		break;
	case kDgain:
		v = DryGain;
		break;
	case kDensity:
		v = ap->properties.flDensity/EAXREVERB_MAX_DENSITY;
		break;
	case kDiffusion:
		v = ap->properties.flDiffusion/EAXREVERB_MAX_DIFFUSION;
		break;
	case kGain:
		v = ap->properties.flGain/EAXREVERB_MAX_GAIN;
		break;
	case kGainHF:
		v = ap->properties.flGainHF/EAXREVERB_MAX_GAINHF;
		break;
	case kGainLF:
		v = ap->properties.flGainLF/EAXREVERB_MAX_GAINLF;
		break;
	case kDecayTime:
		v = ap->properties.flDecayTime/EAXREVERB_MAX_DECAY_TIME;
		break;
	case kDecayHFRatio:
		v = ap->properties.flDecayHFRatio/EAXREVERB_MAX_DECAY_HFRATIO;
		break;
	case kDecayLFRatio:
		v = ap->properties.flDecayLFRatio/EAXREVERB_MAX_DECAY_LFRATIO;
		break;
	case kReflectionsGain:
		v = ap->properties.flReflectionsGain/EAXREVERB_MAX_REFLECTIONS_GAIN;
		break;
	case kReflectionsDelay:
		v = ap->properties.flReflectionsDelay/EAXREVERB_MAX_REFLECTIONS_DELAY;
		break;
	case kReflectionsPanX:
		v = ap->properties.flReflectionsPan[0];
		break;
	case kReflectionsPanY:
		v = ap->properties.flReflectionsPan[1];
		break;
	case kReflectionsPanZ:
		v = ap->properties.flReflectionsPan[2];
		break;
	case kLateReverbGain:
		v = ap->properties.flLateReverbGain/EAXREVERB_MAX_LATE_REVERB_GAIN;
		break;
	case kLateReverbDelay:
		v = ap->properties.flLateReverbDelay/EAXREVERB_MAX_LATE_REVERB_DELAY;
		break;
	case kLateReverbPanX:
		v = ap->properties.flLateReverbPan[0];
		break;
	case kLateReverbPanY:
		v = ap->properties.flLateReverbPan[1];
		break;
	case kLateReverbPanZ:
		v = ap->properties.flLateReverbPan[2];
		break;
	case kEchoTime:
		v = ap->properties.flEchoTime/EAXREVERB_MAX_ECHO_TIME;
		break;
	case kEchoDepth:
		v = ap->properties.flEchoDepth/EAXREVERB_MAX_ECHO_DEPTH;
		break;
	case kModulationTime:
		v = ap->properties.flModulationTime/EAXREVERB_MAX_MODULATION_TIME;
		break;
	case kModulationDepth:
		v = ap->properties.flModulationDepth/EAXREVERB_MAX_MODULATION_DEPTH;
		break;
	case kAirAbsorptionGainHF:
		v = ap->properties.flAirAbsorptionGainHF/EAXREVERB_MAX_AIR_ABSORPTION_GAINHF;
		break;
	case kHFReference:
		v = ap->properties.flHFReference/EAXREVERB_MAX_HFREFERENCE;
		break;
	case kLFReference:
		v = ap->properties.flLFReference/EAXREVERB_MAX_LFREFERENCE;
		break;
	case kRoomRolloffFactor:
		v = ap->properties.flRoomRolloffFactor/EAXREVERB_MAX_ROOM_ROLLOFF_FACTOR;
		break;
	case kDecayHFLimit:
		v = ap->properties.iDecayHFLimit;
		break;
	}
	return v;
}

void EAXReverb::getParameterDisplay (VstInt32 index, char *text)
{
	EAXReverbProgram* ap = &programs[curProgram];
	switch (index)
	{
	case kDisable:
		if (DisableEffect >= 0.5)
		{
			strcpy (text, "ON");
		}
		else
		{
			strcpy (text, "OFF");
		}
		break;
	case kDgain:
		float2string (DryGain, text, kVstMaxParamStrLen);
		break;
	case kDensity:
		float2string (ap->properties.flDensity, text, kVstMaxParamStrLen);
		break;
	case kDiffusion:
		float2string (ap->properties.flDiffusion, text, kVstMaxParamStrLen);
		break;
	case kGain:
		float2string (ap->properties.flGain, text, kVstMaxParamStrLen);
		break;
	case kGainHF:
		float2string (ap->properties.flGainHF, text, kVstMaxParamStrLen);
		break;
	case kGainLF:
		float2string (ap->properties.flGainLF, text, kVstMaxParamStrLen);
		break;
	case kDecayTime:
		float2string (ap->properties.flDecayTime, text, kVstMaxParamStrLen);
		break;
	case kDecayHFRatio:
		float2string (ap->properties.flDecayHFRatio, text, kVstMaxParamStrLen);
		break;
	case kDecayLFRatio:
		float2string (ap->properties.flDecayLFRatio, text, kVstMaxParamStrLen);
		break;
	case kReflectionsGain:
		float2string (ap->properties.flReflectionsGain, text, kVstMaxParamStrLen);
		break;
	case kReflectionsDelay:
		float2string (ap->properties.flReflectionsDelay, text, kVstMaxParamStrLen);
		break;
	case kReflectionsPanX:
		float2string (ap->properties.flReflectionsPan[0], text, kVstMaxParamStrLen);
		break;
	case kReflectionsPanY:
		float2string (ap->properties.flReflectionsPan[1], text, kVstMaxParamStrLen);
		break;
	case kReflectionsPanZ:
		float2string (ap->properties.flReflectionsPan[2], text, kVstMaxParamStrLen);
		break;
	case kLateReverbGain:
		float2string (ap->properties.flLateReverbGain, text, kVstMaxParamStrLen);
		break;
	case kLateReverbDelay:
		float2string (ap->properties.flLateReverbDelay, text, kVstMaxParamStrLen);
		break;
	case kLateReverbPanX:
		float2string (ap->properties.flLateReverbPan[0], text, kVstMaxParamStrLen);
		break;
	case kLateReverbPanY:
		float2string (ap->properties.flLateReverbPan[1], text, kVstMaxParamStrLen);
		break;
	case kLateReverbPanZ:
		float2string (ap->properties.flLateReverbPan[2], text, kVstMaxParamStrLen);
		break;
	case kEchoTime:
		float2string (ap->properties.flEchoTime, text, kVstMaxParamStrLen);
		break;
	case kEchoDepth:
		float2string (ap->properties.flEchoDepth, text, kVstMaxParamStrLen);
		break;
	case kModulationTime:
		float2string (ap->properties.flModulationTime, text, kVstMaxParamStrLen);
		break;
	case kModulationDepth:
		float2string (ap->properties.flModulationDepth, text, kVstMaxParamStrLen);
		break;
	case kAirAbsorptionGainHF:
		float2string (ap->properties.flAirAbsorptionGainHF, text, kVstMaxParamStrLen);
		break;
	case kHFReference:
		float2string (ap->properties.flHFReference, text, kVstMaxParamStrLen);
		break;
	case kLFReference:
		float2string (ap->properties.flLFReference, text, kVstMaxParamStrLen);
		break;
	case kRoomRolloffFactor:
		float2string (ap->properties.flRoomRolloffFactor, text, kVstMaxParamStrLen);
		break;
	case kDecayHFLimit:
		if (ap->properties.iDecayHFLimit)
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

void EAXReverb::getParameterLabel (VstInt32 index, char *label)
{
	switch (index)
	{
	case kDgain:
		strcpy (label, "F");
		break;
	case kDensity:
		strcpy (label, "F");
		break;
	case kDiffusion:
		strcpy (label, "F");
		break;
	case kGain:
		strcpy (label, "F");
		break;
	case kGainHF:
		strcpy (label, "F");
		break;
	case kGainLF:
		strcpy (label, "F");
		break;
	case kDecayTime:
		strcpy (label, "Seconds");
		break;
	case kDecayHFRatio:
		strcpy (label, "F");
		break;
	case kDecayLFRatio:
		strcpy (label, "F");
		break;
	case kReflectionsGain:
		strcpy (label, "F");
		break;
	case kReflectionsDelay:
		strcpy (label, "Seconds");
		break;
	case kReflectionsPanX:
		strcpy (label, "F");
		break;
	case kReflectionsPanY:
		strcpy (label, "F");
		break;
	case kReflectionsPanZ:
		strcpy (label, "F");
		break;
	case kLateReverbGain:
		strcpy (label, "F");
		break;
	case kLateReverbDelay:
		strcpy (label, "Seconds");
		break;
	case kLateReverbPanX:
		strcpy (label, "F");
		break;
	case kLateReverbPanY:
		strcpy (label, "F");
		break;
	case kLateReverbPanZ:
		strcpy (label, "F");
		break;
	case kEchoTime:
		strcpy (label, "Seconds");
		break;
	case kEchoDepth:
		strcpy (label, "F");
		break;
	case kModulationTime:
		strcpy (label, "Seconds");
		break;
	case kModulationDepth:
		strcpy (label, "F");
		break;
	case kAirAbsorptionGainHF:
		strcpy (label, "F");
		break;
	case kHFReference:
		strcpy (label, "Hz");
		break;
	case kLFReference:
		strcpy (label, "Hz");
		break;
	case kRoomRolloffFactor:
		strcpy (label, "F");
		break;
	}
}

void EAXReverb::getParameterName (VstInt32 index, char *text)
{
	switch (index)
	{
	case kDisable:
		strcpy (text, "DisableEffect");
		break;
	case kDgain:
		strcpy (text, "DryGain");
		break;
	case kDensity:
		strcpy (text, "Density");
		break;
	case kDiffusion:
		strcpy (text, "Diffusion");
		break;
	case kGain:
		strcpy (text, "Gain");
		break;
	case kGainHF:
		strcpy (text, "GainHF");
		break;
	case kGainLF:
		strcpy (text, "GainLF");
		break;
	case kDecayTime:
		strcpy (text, "DecayTime");
		break;
	case kDecayHFRatio:
		strcpy (text, "DecayHFRatio");
		break;
	case kDecayLFRatio:
		strcpy (text, "DecayLFRatio");
		break;
	case kReflectionsGain:
		strcpy (text, "ReflectionsGain");
		break;
	case kReflectionsDelay:
		strcpy (text, "ReflectionsDelay");
		break;
	case kReflectionsPanX:
		strcpy (text, "ReflectionsPanX");
		break;
	case kReflectionsPanY:
		strcpy (text, "ReflectionsPanY");
		break;
	case kReflectionsPanZ:
		strcpy (text, "ReflectionsPanZ");
		break;
	case kLateReverbGain:
		strcpy (text, "LateReverbGain");
		break;
	case kLateReverbDelay:
		strcpy (text, "LateReverbDelay");
		break;
	case kLateReverbPanX:
		strcpy (text, "LateReverbPanX");
		break;
	case kLateReverbPanY:
		strcpy (text, "LateReverbPanY");
		break;
	case kLateReverbPanZ:
		strcpy (text, "LateReverbPanZ");
		break;
	case kEchoTime:
		strcpy (text, "EchoTime");
		break;
	case kEchoDepth:
		strcpy (text, "EchoDepth");
		break;
	case kModulationTime:
		strcpy (text, "ModulationTime");
		break;
	case kModulationDepth:
		strcpy (text, "ModulationDepth");
		break;
	case kAirAbsorptionGainHF:
		strcpy (text, "AirAbsorptionGainHF");
		break;
	case kHFReference:
		strcpy (text, "HFReference");
		break;
	case kLFReference:
		strcpy (text, "LFReference");
		break;
	case kRoomRolloffFactor:
		strcpy (text, "RoomRolloffFactor");
		break;
	case kDecayHFLimit:
		strcpy (text, "DecayHFLimit");
		break;
	}
}

void EAXReverb::setProgram (VstInt32 program)
{
	curProgram = program;
	UpdateEffect();
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

VstInt32 EAXReverb::getVendorVersion ()
{
	return 1000;
}

VstPlugCategory EAXReverb::getPlugCategory ()
{
	return kPlugCategEffect;
}
