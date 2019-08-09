
#ifndef __eaxreverb__
#include "eaxreverb.h"
#endif


AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new eaxreverb (audioMaster);
}

