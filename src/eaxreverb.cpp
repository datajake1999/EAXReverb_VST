
#include <stdio.h>
#include <string.h>

#ifndef __eaxreverb__
#include "eaxreverb.h"
#endif


eaxreverbProgram::eaxreverbProgram ()
{
}


eaxreverb::eaxreverb (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, kNumPrograms, kNumParams)
{
	// init
	rate = (int)sampleRate;
	effect.Create(rate);
	effect.Update(rate);
	
	programs = new eaxreverbProgram[numPrograms];

	if (programs)
	for (unsigned int i = 0; i < kNumPrograms; i++)
	{
		setProgram(i);
		setProgramName(GetPresetName(i));
	}
	setProgram (0);

	SetInvertReverb (0);
	SetMonoReverb (0);
	SetOnlyReverb (0);
	SetDryGain (1);
	SetWetGain (1);

	setNumInputs (2);	// stereo input
	setNumOutputs (2);	// stereo output
	setUniqueID ('EAX');

}


eaxreverb::~eaxreverb ()
{
	effect.Destroy();
	if (programs)
	delete[] programs;
}


VstInt32 eaxreverb::getProgram ()
{
	return curProgram;
}


void eaxreverb::setProgram (VstInt32 program)
{
	curProgram = program;
	SetReverbPreset(program);
}


void eaxreverb::SetReverbPreset(int preset) {
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
}

char *eaxreverb::GetPresetName(int preset) {
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

void eaxreverb::SetInvertReverb (float val)
{
	InvertReverb = val;
}


void eaxreverb::SetMonoReverb (float val)
{
	MonoReverb = val;
}


void eaxreverb::SetOnlyReverb (float val)
{
	OnlyReverb = val;
}


void eaxreverb::SetDryGain (float val)
{
	DryGain = val;
}


void eaxreverb::SetWetGain (float val)
{
	WetGain = val;
}


void eaxreverb::SetDensity (float val)
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


void eaxreverb::SetDiffusion (float val)
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


void eaxreverb::SetGain (float val)
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


void eaxreverb::SetGainHF (float val)
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


void eaxreverb::SetGainLF (float val)
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


void eaxreverb::SetDecayTime (float val)
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


void eaxreverb::SetDecayHFRatio (float val)
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


void eaxreverb::SetDecayLFRatio (float val)
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


void eaxreverb::SetReflectionsGain (float val)
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


void eaxreverb::SetReflectionsDelay (float val)
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


void eaxreverb::SetReflectionsPanX (float val)
{
	ReflectionsPanX = val;
	programs[curProgram].ReflectionsPanX = val;
}


void eaxreverb::SetReflectionsPanY (float val)
{
	ReflectionsPanY = val;
	programs[curProgram].ReflectionsPanY = val;
}


void eaxreverb::SetReflectionsPanZ (float val)
{
	ReflectionsPanZ = val;
	programs[curProgram].ReflectionsPanZ = val;
}


void eaxreverb::SetLateReverbGain (float val)
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


void eaxreverb::SetLateReverbDelay (float val)
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


void eaxreverb::SetLateReverbPanX (float val)
{
	LateReverbPanX = val;
	programs[curProgram].LateReverbPanX = val;
}


void eaxreverb::SetLateReverbPanY (float val)
{
	LateReverbPanY = val;
	programs[curProgram].LateReverbPanY = val;
}


void eaxreverb::SetLateReverbPanZ (float val)
{
	LateReverbPanZ = val;
	programs[curProgram].LateReverbPanZ = val;
}


void eaxreverb::SetEchoTime (float val)
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


void eaxreverb::SetEchoDepth (float val)
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


void eaxreverb::SetModulationTime (float val)
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


void eaxreverb::SetModulationDepth (float val)
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


void eaxreverb::SetAirAbsorptionGainHF (float val)
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


void eaxreverb::SetHFReference (float val)
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


void eaxreverb::SetLFReference (float val)
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


void eaxreverb::SetRoomRolloffFactor (float val)
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


void eaxreverb::SetDecayHFLimit (float val)
{
	DecayHFLimit = val;
	programs[curProgram].DecayHFLimit = val;
	if (DecayHFLimit >= 0.5)
	{
		i_DecayHFLimit = 1;
	}
	else
	{
		i_DecayHFLimit = 0;
	}
}


void eaxreverb::SetEAXParams(float Density, float Diffusion, float Gain, float GainHF, float GainLF, float DecayTime, float DecayHFRatio, float DecayLFRatio, float ReflectionsGain, float ReflectionsDelay, float ReflectionsPanX, float ReflectionsPanY, float ReflectionsPanZ, float LateReverbGain, float LateReverbDelay, float LateReverbPanX, float LateReverbPanY, float LateReverbPanZ, float EchoTime, float EchoDepth, float ModulationTime, float ModulationDepth, float AirAbsorptionGainHF, float HFReference, float LFReference, float RoomRolloffFactor, float DecayHFLimit)
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
	effect.LoadPreset(Density, Diffusion, Gain, GainHF, GainLF, DecayTime, DecayHFRatio, DecayLFRatio, ReflectionsGain, ReflectionsDelay, ReflectionsPanX, ReflectionsPanY, ReflectionsPanZ, LateReverbGain, LateReverbDelay, LateReverbPanX, LateReverbPanY, LateReverbPanZ, EchoTime, EchoDepth, ModulationTime, ModulationDepth, AirAbsorptionGainHF, HFReference, LFReference, RoomRolloffFactor, i_DecayHFLimit);
	effect.Update(rate);
}


void eaxreverb::setProgramName (char *name)
{
	strcpy (programs[curProgram].name, name);
}


void eaxreverb::getProgramName (char *name)
{
	strcpy (name, programs[curProgram].name);
}

bool eaxreverb::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
	if (index < kNumPrograms)
	{
		strcpy (text, programs[index].name);
		return true;
	}
	return false;
}


void eaxreverb::setParameter (VstInt32 index, float value)
{
	switch (index)
	{
	case kInvertrev :    SetInvertReverb (value);					break;
	case kMonorev :    SetMonoReverb (value);					break;
	case kOnlyrev :    SetOnlyReverb (value);					break;
	case kDgain :    SetDryGain (value);					break;
	case kWgain :    SetWetGain (value);					break;
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
	if (index > kWgain && index < kNumParams)
	{
		effect.LoadPreset(Density, Diffusion, Gain, GainHF, GainLF, DecayTime, DecayHFRatio, DecayLFRatio, ReflectionsGain, ReflectionsDelay, ReflectionsPanX, ReflectionsPanY, ReflectionsPanZ, LateReverbGain, LateReverbDelay, LateReverbPanX, LateReverbPanY, LateReverbPanZ, EchoTime, EchoDepth, ModulationTime, ModulationDepth, AirAbsorptionGainHF, HFReference, LFReference, RoomRolloffFactor, i_DecayHFLimit);
		effect.Update(rate);
	}
}


void eaxreverb::setParameterAutomated (VstInt32 index, float value)
{
	setParameter(index, value);
}


float eaxreverb::getParameter (VstInt32 index)
{
	float v = 0;

	switch (index)
	{
	case kInvertrev :    v = InvertReverb;	break;
	case kMonorev :    v = MonoReverb;	break;
	case kOnlyrev :    v = OnlyReverb;	break;
	case kDgain :    v = DryGain;	break;
	case kWgain :    v = WetGain;	break;
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


void eaxreverb::getParameterLabel (VstInt32 index, char *label)
{
	switch (index)
	{
	case kDgain :    strcpy (label, "F");		break;
	case kWgain :    strcpy (label, "F");		break;
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


void eaxreverb::getParameterName (VstInt32 index, char *text)
{
	switch (index)
	{
	case kInvertrev :    strcpy (text, "InvertReverb");		break;
	case kMonorev :    strcpy (text, "MonoReverb");		break;
	case kOnlyrev :    strcpy (text, "OnlyReverb");		break;
	case kDgain :    strcpy (text, "DryGain");		break;
	case kWgain :    strcpy (text, "WetGain");		break;
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


void eaxreverb::getParameterDisplay (VstInt32 index, char *text)
{
	switch (index)
	{
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
	case kDgain : float2string (DryGain, text, kVstMaxParamStrLen);	break;
	case kWgain : float2string (WetGain, text, kVstMaxParamStrLen);	break;
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


bool eaxreverb::getEffectName (char* name)
{
	strcpy (name, "EAXReverb");
	return true;
}


bool eaxreverb::getProductString (char* text)
{
	strcpy (text, "EAXReverb");
	return true;
}


bool eaxreverb::getVendorString (char* text)
{
	strcpy (text, "Datajake");
	return true;
}


void eaxreverb::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	float* in1 = inputs[0];
	float* in2 = inputs[1];
	float* out1 = outputs[0];
	float* out2 = outputs[1];
	//check the sample rate, since the effect has issues when working with sample rates below 10000 HZ
	if (rate < 10000)
	{
		return;
	}
	//allocate memory for mono samples
	float *floatSamplesIn =  new float[sampleFrames];
	int i;
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
		//invert the phase of the reverb if we set InvertReverb to true
		if (InvertReverb >= 0.5)
		{
			for (i=0; i<workSamples; i++)
			{
				floatSamplesOut[i*2 + 0] = floatSamplesOut[i*2 + 0] * -1;
				floatSamplesOut[i*2 + 1] = floatSamplesOut[i*2 + 1] * -1;
			}
		}
		//mixdown the reverb to mono if we set MonoReverb to true
		if (MonoReverb >= 0.5)
		{
			for (i=0; i<workSamples; i++)
			{
				float sample = (floatSamplesOut[i*2 + 0] + floatSamplesOut[i*2 + 1]) / 2;
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
			//check if we are only generating the reverb output
			if (OnlyReverb >= 0.5)
			{
				*out1 = floatSamplesOut[i*2 + 0];
				*out2 = floatSamplesOut[i*2 + 1];
			}
			else
			{
				*out1 = *in1 + floatSamplesOut[i*2 + 0];
				*out2 = *in2 + floatSamplesOut[i*2 + 1];
			}
			*in1++;
			*in2++;
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
